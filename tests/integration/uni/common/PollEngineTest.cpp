#include <chrono>

#include "uni/common/PollEngine.hpp"
#include "uni/common/Runtime.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace integration
{
namespace uni
{
namespace common
{
class PollEngineTest : public ::testing::Test
{
};

}  // namespace common
}  // namespace uni
}  // namespace integration

using integration::uni::common::PollEngineTest;

TEST_F( PollEngineTest, test_poll_engine_callback )
{
    auto runtime = std::make_shared< uni::common::Runtime >( );
    runtime->task_dispatcher( ).start( );

    {
        uni::common::PollEngine poll_engine( runtime );

        constexpr uni::TimeInterval POLL_INTERVAL_MS = 100;
        poll_engine.set_poll_interval( POLL_INTERVAL_MS );

        static const auto START_TIME = std::chrono::steady_clock::now( );

        auto poll_intervals_from_start_ptr = std::make_shared< std::vector< size_t > >( );
        auto callback = [poll_intervals_from_start_ptr]( ) {
            auto now = std::chrono::steady_clock::now( );
            const auto timeout_ms
                = std::chrono::duration_cast< std::chrono::milliseconds >( now - START_TIME );

            auto& poll_intervals_from_start = *poll_intervals_from_start_ptr;
            poll_intervals_from_start.push_back( timeout_ms.count( ) );
        };
        poll_engine.set_poll_callback( std::move( callback ) );
        poll_engine.start( );

        constexpr size_t MAX_POLL_COUNTER = 5;
        constexpr auto WAIT_MS = MAX_POLL_COUNTER * POLL_INTERVAL_MS;
        std::this_thread::sleep_for( std::chrono::milliseconds( WAIT_MS ) );

        poll_engine.stop( );

        auto& poll_intervals_from_start = *poll_intervals_from_start_ptr;
        ASSERT_EQ( MAX_POLL_COUNTER, poll_intervals_from_start.size( ) );

        constexpr int64_t DELTA_MS = 10;
        for ( size_t i = 0; i < poll_intervals_from_start.size( ); ++i )
        {
            const auto poll_interval_from_start_ms = poll_intervals_from_start[ i ];
            //            std::cout << "poll_interval_from_start_ms=" << poll_interval_from_start_ms
            //            << '\n';
            const auto poll_counter = i + 1;
            const int64_t diff = std::abs( static_cast< int64_t >(
                poll_interval_from_start_ms - POLL_INTERVAL_MS * poll_counter ) );

            ASSERT_LE( diff, DELTA_MS );
        }
    }
    runtime->task_dispatcher( ).stop( );
}
