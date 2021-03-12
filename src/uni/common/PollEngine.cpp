#include "uni/common/PollEngine.hpp"

namespace uni
{
namespace common
{
PollEngine::PollEngine( const Runtime& runtime, const std::string& thread_pool_name )
    : m_runtime{runtime}
    , m_thread_pool_name{thread_pool_name}
{
}

void
PollEngine::start( )
{
    m_running = true;
    run( );
}

void
PollEngine::stop( )
{
    m_running = false;
    m_runtime.task_dispatcher_basic( ).cancel_pending( m_thread_pool_name,
                                                       m_poll_request_id.load( ) );
}

void
PollEngine::set_poll_interval( TimeInterval poll_interval_ms )
{
    m_poll_interval_ms = poll_interval_ms;
}

void
PollEngine::poll( )
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
    run( );
}

void
PollEngine::run( )
{
    auto& dispatcher = m_runtime.task_dispatcher_basic( );
    auto task = uni::create_task_ptr( this, &PollEngine::poll );
    task->set_due_time( m_poll_interval_ms );
    m_poll_request_id = utils::create_request_id( );

    dispatcher.dispatch( m_thread_pool_name, m_poll_request_id.load( ), std::move( task ) );
}

}  // namespace common
}  // namespace uni
