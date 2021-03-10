#include "uni/common/DemoComponentImpl.hpp"
#include "uni/common/DemoComponentListener.hpp"
#include "uni/common/Runtime.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace integration
{
namespace uni
{
namespace common
{
class DemoComponentTest : public ::testing::Test
{
};

}  // namespace common
}  // namespace uni
}  // namespace integration

class TestDemoComponentListener : public ::integration::uni::common::DemoComponentListener
{
public:
    explicit TestDemoComponentListener( uni::common::Runtime& runtime )
        : m_runtime{runtime}
        , m_calculation_done_wait( m_calculation_done_notify.get_future( ) )
        , m_calculation_started_wait( m_calculation_started_notify.get_future( ) )
    {
    }

public:
    void
    wait_on_calculation_started( )
    {
        ASSERT_TRUE( m_calculation_started_wait.valid( ) );
        m_calculation_started_wait.wait( );
    }

    void
    wait_on_calculation_done( )
    {
        ASSERT_TRUE( m_calculation_done_wait.valid( ) );
        m_calculation_done_wait.wait( );
    }

public:
    void
    on_calculate_started( uni::RequestId id ) override
    {
        m_runtime.task_dispatcher( ).assert_execution_context( "client" );

        UNUSED( id );

        m_calculation_started_notify.set_value( );
    }

    void
    on_calculate_progress( uni::RequestId id, size_t progress ) override
    {
        m_runtime.task_dispatcher( ).assert_execution_context( "client" );

        UNUSED( id );
        UNUSED( progress );
    }

    void
    on_calculation_done( uni::RequestId id,
                         int result1,
                         const std::string& result2,
                         bool status ) override
    {
        m_runtime.task_dispatcher( ).assert_execution_context( "client" );

        m_id = id;
        m_result1 = result1;
        m_result2 = result2;
        m_status = status;

        m_calculation_done_notify.set_value( );
    }

private:
    uni::common::Runtime& m_runtime;
    std::promise< void > m_calculation_done_notify;
    std::future< void > m_calculation_done_wait;
    std::promise< void > m_calculation_started_notify;
    std::future< void > m_calculation_started_wait;

public:
    uni::RequestId m_id{0};
    int m_result1{0};
    std::string m_result2;
    bool m_status{false};
};

using integration::uni::common::DemoComponentTest;

TEST_F( DemoComponentTest, test_demo_component_calculate_1 )
{
    using integration::uni::common::DemoComponentImpl;

    uni::common::Runtime runtime;
    runtime.task_dispatcher( ).start( );

    TestDemoComponentListener listener{runtime};
    const auto component = std::make_shared< DemoComponentImpl >( runtime, listener );
    auto request = component->calculate_1_impl( 2, "hello" );
    const uni::RequestId id = request.get_request_id( );
    request.start( );

    listener.wait_on_calculation_done( );

    runtime.task_dispatcher( ).stop( );

    ASSERT_EQ( listener.m_id, id );
    ASSERT_EQ( listener.m_result1, 2 );
    ASSERT_EQ( listener.m_result2, "hello" );
    ASSERT_EQ( listener.m_status, true );
}

TEST_F( DemoComponentTest, test_demo_component_cancel_calculate_1 )
{
    using integration::uni::common::DemoComponentImpl;

    uni::common::Runtime runtime;
    runtime.task_dispatcher( ).start( );

    TestDemoComponentListener listener{runtime};
    const auto component = std::make_shared< DemoComponentImpl >( runtime, listener );
    // 10*15 iterations 100ms each
    auto request = component->calculate_1_impl( 10 * 15, "hello" );
    const uni::RequestId id = request.get_request_id( );
    request.start( );

    listener.wait_on_calculation_started( );

    request.cancel( );

    listener.wait_on_calculation_done( );

    runtime.task_dispatcher( ).stop( );

    ASSERT_EQ( listener.m_id, id );
    ASSERT_EQ( listener.m_result1, 10 * 15 );
    ASSERT_EQ( listener.m_result2, "hello" );
    ASSERT_EQ( listener.m_status, false );
}
