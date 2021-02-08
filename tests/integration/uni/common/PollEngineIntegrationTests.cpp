#include <chrono>

#include "uni/common/PollEngine.hpp"
#include "uni/common/Runtime.hpp"

#define ASSERT( x )                                                                  \
    do                                                                               \
    {                                                                                \
        if ( !( x ) )                                                                \
        {                                                                            \
            std::fprintf( stderr, "Assertion failed: %s:%i\n", __FILE__, __LINE__ ); \
            std::fflush( stderr );                                                   \
        }                                                                            \
    } while ( 0 )

#define ASSERT_EQ( lhs, rhs ) ASSERT( lhs == rhs )

#define ASSERT_LE( lhs, rhs ) ASSERT( lhs <= rhs )

void
test_poll_engine_callback( )
{
    uni::common::Runtime runtime;
    runtime.task_dispatcher( ).start( );

    {
        uni::common::PollEngine poll_engine( &runtime );

        // TODO: use string literals
        constexpr uni::TimeInterval POLL_INTERVAL_MS = 100;
        poll_engine.set_poll_interval( POLL_INTERVAL_MS );

        std::vector< size_t > call_timeout_intervals;

        static auto start_time = std::chrono::steady_clock::now( );

        std::atomic< size_t > counter{0};
        auto callback = [&counter, &call_timeout_intervals]( ) mutable {
            ++counter;
            auto now = std::chrono::steady_clock::now( );
            const auto timeout_ms
                = std::chrono::duration_cast< std::chrono::milliseconds >( now - start_time );
            call_timeout_intervals.push_back( timeout_ms.count( ) );

            //            std::thread::id this_id = std::this_thread::get_id( );
            //            std::cout << "timeout_ms=" << timeout_ms.count( ) << ", thread_id2=" <<
            //            this_id
            //                      << ", counter=" << counter << '\n';
        };
        poll_engine.set_poll_callback( std::move( callback ) );
        poll_engine.start( );

        using namespace std::chrono_literals;
        std::this_thread::sleep_for( 500ms );
        poll_engine.stop( );

        ASSERT_EQ( counter, 5 );

        constexpr int64_t DELTA_MS = 5;
        for ( size_t i = 0; i < call_timeout_intervals.size( ); ++i )
        {
            const auto timeout = call_timeout_intervals[ i ];
            const int64_t diff
                = std::abs( static_cast< int64_t >( timeout - POLL_INTERVAL_MS * ( i + 1 ) ) );
            ASSERT_LE( diff, static_cast< int64_t >( DELTA_MS * ( i + 1 ) ) );
        }
    }
    runtime.task_dispatcher( ).stop( );
}

void
poll_engine_tests( )
{
    test_poll_engine_callback( );
}
