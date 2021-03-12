#include "uni/common/PollEngineImpl.hpp"

namespace uni
{
namespace common
{
PollEngineImpl::PollEngineImpl( const Runtime& runtime, const std::string& thread_pool_name )
    : m_runtime{runtime}
    , m_thread_pool_name{thread_pool_name}
{
}

void
PollEngineImpl::start_impl( )
{
    m_running = true;
    run_impl( );
}

void
PollEngineImpl::stop_impl( )
{
    m_running = false;
    m_runtime.task_dispatcher_basic( ).cancel_pending( m_thread_pool_name,
                                                       m_poll_request_id.load( ) );
}

void
PollEngineImpl::set_poll_interval_impl( TimeInterval poll_interval_ms )
{
    m_poll_interval_ms = poll_interval_ms;
}

void
PollEngineImpl::set_poll_callback_impl( Callback callback )
{
    m_poll_callback = std::move( callback );
}

void
PollEngineImpl::poll_impl( )
{
    m_runtime.task_dispatcher_basic( ).assert_execution_context( m_thread_pool_name );

    if ( !m_running )
    {
        return;
    }

    if ( m_poll_callback )
    {
        m_poll_callback( );
    }
    run_impl( );
}

void
PollEngineImpl::run_impl( )
{
    auto& dispatcher = m_runtime.task_dispatcher_basic( );
    auto task = uni::create_task_ptr( this, &PollEngineImpl::poll_impl );
    task->set_due_time( m_poll_interval_ms );
    m_poll_request_id = utils::create_request_id( );

    dispatcher.dispatch( m_thread_pool_name, m_poll_request_id.load( ), std::move( task ) );
}

}  // namespace common
}  // namespace uni
