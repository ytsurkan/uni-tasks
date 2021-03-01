#include "uni/common/PollEngine.hpp"

#include "uni/common/Runtime.hpp"

namespace uni
{
namespace common
{
PollEngine::PollEngine( std::shared_ptr< Runtime > runtime, const std::string& thread_pool_name )
    : m_runtime{std::move( runtime )}
    , m_thread_pool_name{thread_pool_name}
{
    if ( nullptr == m_runtime )
    {
        throw std::logic_error( "Invalid runtime parameter" );
    }
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
    m_runtime->task_dispatcher( ).cancel_pending( m_thread_pool_name, m_poll_request_id.load( ) );
}

void
PollEngine::set_poll_interval( TimeInterval poll_interval_ms )
{
    m_poll_interval_ms = poll_interval_ms;
}

void
PollEngine::poll( )
{
    m_runtime->task_dispatcher( ).assert_execution_context( m_thread_pool_name );

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
    auto& dispatcher = m_runtime->task_dispatcher( );
    auto task = uni::create_task_ptr( this, &PollEngine::poll );
    task->set_due_time( m_poll_interval_ms );
    m_poll_request_id = utils::create_request_id( );

    dispatcher.dispatch( m_thread_pool_name, m_poll_request_id.load( ), std::move( task ) );
}

}  // namespace common
}  // namespace uni
