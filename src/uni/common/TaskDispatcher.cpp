#include "uni/common/TaskDispatcher.hpp"

namespace uni
{
TaskDispatcher::TaskDispatcher(
    const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations )
    : m_impl{thread_pool_configurations}
{
}
void
TaskDispatcher::start( )
{
    m_impl.start_impl( );
}

void
TaskDispatcher::stop( )
{
    m_impl.stop_impl( );
}

void
TaskDispatcher::cancel_pending( const std::string& thread_pool_name, RequestId request_id )
{
    m_impl.cancel_pending_impl( thread_pool_name, request_id );
}

void
TaskDispatcher::dispatch( const std::string& thread_pool_name, std::unique_ptr< ITask > task )
{
    m_impl.dispatch_impl( thread_pool_name, std::move( task ) );
}

void
TaskDispatcher::dispatch( const std::string& thread_pool_name,
                          RequestId request_id,
                          std::unique_ptr< ITask > task )
{
    m_impl.dispatch_impl( thread_pool_name, request_id, std::move( task ) );
}

void
TaskDispatcher::assert_execution_context( const std::string& thread_pool_name )
{
    m_impl.assert_execution_context_impl( thread_pool_name );
}
}  // namespace uni
