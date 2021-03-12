#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "uni/common/TaskDispatcher.hpp"

namespace unit
{
namespace uni
{
namespace common
{
class TaskDispatcherTest : public ::testing::Test
{
};

}  // namespace common
}  // namespace uni
}  // namespace unit

using unit::uni::common::TaskDispatcherTest;

namespace
{
struct FakeWorker
{
    static constexpr size_t s_magic_number = 13;

    void
    do_1( int32_t value )
    {
        m_result = value;
    }

    void
    do_2( int32_t value1, int32_t value2 )
    {
        m_result = value1 + value2;
    }

    void
    do_2( int32_t value1, int32_t value2 ) const
    {
        m_result = value1 + value2 + s_magic_number;
    }
    mutable int32_t m_result{};
};

void
sum( int32_t first, int32_t second, std::shared_ptr< int32_t > sum )
{
    *sum = first + second;
}
}  // namespace

TEST_F( TaskDispatcherTest, test_execute_lambda )
{
    const auto f = []( int32_t a, int32_t b ) {
        const int32_t value = a + b;
        const int32_t expected = 2 + 3;
        ASSERT_EQ( value, expected );
    };

    uni::common::TaskDispatcher dispatcher;
    dispatcher.start( );

    bool async_executed{false};
    const auto call_wrapper = [&]( int x, int y ) {
        const bool executed = dispatcher.dispatch_or_execute( "background", f, x, y );
        ASSERT_EQ( executed, true );
        async_executed = true;
    };

    const int32_t a = 2;
    const bool sync_executed = dispatcher.dispatch_or_execute( "background", call_wrapper, a, 3 );
    dispatcher.stop( );

    ASSERT_EQ( sync_executed, false );
    ASSERT_EQ( async_executed, true );
}

TEST_F( TaskDispatcherTest, test_dispatch_lambda )
{
    std::promise< bool > async_executed_push;
    std::future< bool > async_executed_pop = async_executed_push.get_future( );
    auto f = [promise = std::move( async_executed_push )]( int32_t, int32_t ) mutable {
        promise.set_value( true );
    };

    uni::common::TaskDispatcher dispatcher;
    dispatcher.start( );

    const bool sync_executed
        = dispatcher.dispatch_or_execute( "background", std::move( f ), 10, 10 );
    ASSERT_EQ( sync_executed, false );

    dispatcher.stop( );

    ASSERT_EQ( sync_executed, false );
    ASSERT_EQ( async_executed_pop.get( ), true );
}

TEST_F( TaskDispatcherTest, test_execute_function )
{
    auto result = std::make_shared< int32_t >( );
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );

        bool async_executed{false};
        const auto call_wrapper = [&async_executed, &dispatcher](
                                      int32_t x, int32_t y, std::shared_ptr< int32_t >& result ) {
            const bool executed = dispatcher.dispatch_or_execute( "background", sum, x, y, result );
            ASSERT_EQ( executed, true );
            async_executed = executed;
        };

        const int32_t a = 1;
        const int32_t b = 2;
        dispatcher.dispatch_or_execute( "background", call_wrapper, a, b, result );
        dispatcher.stop( );

        ASSERT_EQ( async_executed, true );
    }

    const int32_t expected = ( 1 + 2 );
    ASSERT_EQ( expected, *result );
}

TEST_F( TaskDispatcherTest, test_dispatch_function )
{
    auto result = std::make_shared< int32_t >( );
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );

        const bool executed = dispatcher.dispatch_or_execute( "background", sum, 1, 2, result );
        ASSERT_EQ( executed, false );

        dispatcher.stop( );
    }

    const int expected = ( 1 + 2 );
    ASSERT_EQ( expected, *result );
}

TEST_F( TaskDispatcherTest, test_execute_memfn )
{
    FakeWorker foo;
    const int32_t expected = 42;
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );

        auto call_wrapper = [&]( int a ) {
            const bool executed
                = dispatcher.dispatch_or_execute( "background", &foo, &FakeWorker::do_1, a );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", call_wrapper, expected );
        dispatcher.stop( );
    }

    ASSERT_EQ( expected, foo.m_result );
}

TEST_F( TaskDispatcherTest, test_dispatch_memfn )
{
    FakeWorker foo1;
    const int32_t expected1 = 42;
    FakeWorker foo2;
    const int32_t expected2 = 13;
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );

        const bool executed
            = dispatcher.dispatch_or_execute( "background", &foo1, &FakeWorker::do_1, expected1 );
        ASSERT_EQ( executed, false );

        dispatcher.dispatch( "background", &foo2, &FakeWorker::do_1, expected2 );
        dispatcher.stop( );
    }

    ASSERT_EQ( expected1, foo1.m_result );
    ASSERT_EQ( expected2, foo2.m_result );
}

TEST_F( TaskDispatcherTest, test_execute_const_memfn )
{
    FakeWorker foo;
    const int32_t a = 42;
    const int32_t b = 1;
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );
        auto call_wrapper = [&]( int x, int y ) {
            const bool executed = dispatcher.dispatch_or_execute(
                "background",
                &foo,
                uni::MemFnPtrWrapper< void ( FakeWorker::* )( int32_t, int32_t ) const >(
                    &FakeWorker::do_2 ),
                x,
                y );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", call_wrapper, a, b );
        dispatcher.stop( );
    }

    const int32_t expected = ( a + b + FakeWorker::s_magic_number );
    ASSERT_EQ( expected, foo.m_result );
}

TEST_F( TaskDispatcherTest, test_dispatch_const_memfn )
{
    FakeWorker foo1;
    FakeWorker foo2;
    const int32_t a = 42;
    const int32_t b = 1;
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );

        const bool executed = dispatcher.dispatch_or_execute(
            "background",
            &foo1,
            uni::MemFnPtrWrapper< void ( FakeWorker::* )( int32_t, int32_t ) const >(
                &FakeWorker::do_2 ),
            a,
            b );
        ASSERT_EQ( executed, false );

        dispatcher.dispatch(
            "background",
            &foo2,
            uni::MemFnPtrWrapper< void ( FakeWorker::* )( int32_t, int32_t ) const >(
                &FakeWorker::do_2 ),
            a,
            b );
        dispatcher.stop( );
    }

    const int32_t expected = ( a + b + FakeWorker::s_magic_number );
    ASSERT_EQ( expected, foo1.m_result );
    ASSERT_EQ( expected, foo2.m_result );
}

TEST_F( TaskDispatcherTest, test_execute_memfn_of_sharedptr_object )
{
    const auto sp = std::make_shared< FakeWorker >( );
    const int32_t expected = 42;
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );
        auto call_wrapper = [&]( int x ) {
            const bool executed
                = dispatcher.dispatch_or_execute( "background", sp, &FakeWorker::do_1, x );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", std::move( call_wrapper ), expected );
        dispatcher.stop( );
    }

    ASSERT_EQ( expected, sp->m_result );
}

TEST_F( TaskDispatcherTest, test_dispatch_memfn_of_sharedptr_object )
{
    const auto sp = std::make_shared< FakeWorker >( );
    const int32_t expected = 42;
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );
        const bool executed
            = dispatcher.dispatch_or_execute( "background", sp, &FakeWorker::do_1, expected );
        dispatcher.stop( );
        ASSERT_EQ( executed, false );
    }

    ASSERT_EQ( expected, sp->m_result );
}

TEST_F( TaskDispatcherTest, test_execute_const_memfn_of_sharedptr_object )
{
    const auto sp = std::make_shared< FakeWorker >( );
    const int32_t a = 42;
    const int32_t b = 1;
    {
        uni::common::TaskDispatcher dispatcher;
        dispatcher.start( );
        auto call_wrapper = [&]( int x, int y ) {
            const bool executed = dispatcher.dispatch_or_execute(
                "background",
                sp,
                uni::MemFnPtrWrapper< void ( FakeWorker::* )( int32_t, int32_t ) const >(
                    &FakeWorker::do_2 ),
                x,
                y );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", call_wrapper, a, b );
        dispatcher.stop( );
    }

    const int32_t expected = ( a + b + FakeWorker::s_magic_number );
    ASSERT_EQ( expected, sp->m_result );
}
