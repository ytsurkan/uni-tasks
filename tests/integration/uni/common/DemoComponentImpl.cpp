#include "uni/common/DemoComponentImpl.hpp"

#include "uni/common/DemoComponentListener.hpp"
#include "uni/common/Request.hpp"
#include "uni/common/Runtime.hpp"

namespace integration
{
namespace uni
{
namespace common
{
DemoComponentImpl::DemoComponentImpl( ::uni::common::Runtime& runtime,
                                      DemoComponentListener& listener )
    : m_runtime{runtime}
    , m_listener{listener}
{
}

::uni::common::Request
DemoComponentImpl::calculate_1_impl( int p1, const std::string& p2 )
{
    const auto id = ::uni::utils::create_request_id( );
    auto ptr = weak_ptr( );
    auto start_callback = [ptr, id, p1, p2]( ) -> std::shared_future< void > {
        if ( const auto& sp = ptr.lock( ) )
        {
            return sp->schedule_calculate_1( id, p1, p2 );
        }
        return {};
    };
    auto cancel_callback = [ptr, id]( std::shared_future< void > future ) {
        if ( const auto& sp = ptr.lock( ) )
        {
            sp->schedule_cancel_calculate_1( id, std::move( future ) );
        }
    };
    return ::uni::common::create_request(
        id, std::move( start_callback ), std::move( cancel_callback ) );
}

std::shared_future< void >
DemoComponentImpl::schedule_calculate_1( ::uni::RequestId id, int p1, const std::string& p2 )
{
    auto& dispatcher = m_runtime.task_dispatcher( );
    auto task = ::uni::create_task_ptr( this, &DemoComponentImpl::do_calculate_1, id, p1, p2 );
    auto future = task->get_future( );
    dispatcher.dispatch( "background", id, std::move( task ) );
    return future;
}

void
DemoComponentImpl::do_calculate_1( ::uni::RequestId id, int32_t p1, const std::string& p2 )
{
    m_runtime.task_dispatcher( ).assert_execution_context( "background" );

    m_runtime.task_dispatcher( ).dispatch(
        "client", &m_listener, &DemoComponentListener::on_calculate_started, id );

    auto& dispatcher = m_runtime.task_dispatcher( );
    const bool executed = dispatcher.dispatch_or_execute(
        "background", this, &DemoComponentImpl::do_calculate_1_internal, id, p1 );

    ::uni::utils::Expects(
        executed, "Unexpected execution context of DemoComponentImpl::do_calculate_1_internal" );

    bool status = true;
    const size_t max_counter = p1;
    for ( size_t counter = 0; counter < max_counter; ++counter )
    {
        if ( m_calculate_1_cancelled )
        {
            status = false;
            break;
        }
        poll_calculate_1( );

        m_runtime.task_dispatcher( ).dispatch(
            "client", &m_listener, &DemoComponentListener::on_calculate_progress, id, counter );
    }

    m_runtime.task_dispatcher( ).dispatch(
        "client", &m_listener, &DemoComponentListener::on_calculation_done, id, p1, p2, status );

    reset_calculate_1( );
}

void
DemoComponentImpl::poll_calculate_1( )
{
    for ( size_t iterations_per_poll = 0; iterations_per_poll < m_max_iterations_per_poll;
          ++iterations_per_poll )
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for( 10ms );
    }
}

void
DemoComponentImpl::cancel_calculate_1( )
{
    m_calculate_1_cancelled = true;
}

void
DemoComponentImpl::reset_calculate_1( )
{
    m_calculate_1_cancelled = false;
}

void
DemoComponentImpl::do_calculate_1_internal( ::uni::RequestId id, int p1 )
{
    UNUSED( id );
    UNUSED( p1 );
}

void
DemoComponentImpl::schedule_cancel_calculate_1( ::uni::RequestId id,
                                                std::shared_future< void > task_future )
{
    auto ptr = weak_ptr( );
    auto task = [ptr, task_future = std::move( task_future )]( ) {
        if ( const auto& sp = ptr.lock( ) )
        {
            sp->cancel_calculate_1( );
        }
        if ( task_future.valid( ) )
        {
            task_future.wait( );
        }
    };
    m_runtime.task_dispatcher( ).cancel( "background", id, std::move( task ) );
}

std::weak_ptr< DemoComponentImpl >
DemoComponentImpl::weak_ptr( )
{
    return shared_from_this( );
}

}  // namespace common
}  // namespace uni
}  // namespace integration
