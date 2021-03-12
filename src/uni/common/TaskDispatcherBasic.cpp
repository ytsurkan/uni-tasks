#include "uni/common/TaskDispatcherBasic.hpp"

namespace uni
{
namespace common
{
TaskDispatcherBasic::TaskDispatcherBasic(
    const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations )
    : m_impl{thread_pool_configurations}
{
}

void
TaskDispatcherBasic::start( )
{
    m_impl.start_impl( );
}

void
TaskDispatcherBasic::stop( )
{
    m_impl.stop_impl( );
}

void
TaskDispatcherBasic::cancel_pending( const std::string& thread_pool_name, RequestId request_id )
{
    m_impl.cancel_pending_impl( thread_pool_name, request_id );
}

void
TaskDispatcherBasic::cancel( const std::string& thread_pool_name,
                             RequestId request_id,
                             std::function< void( ) > f )
{
    m_impl.cancel_impl( thread_pool_name, request_id, std::move( f ) );
}

void
TaskDispatcherBasic::dispatch( const std::string& thread_pool_name,
                               RequestId request_id,
                               std::unique_ptr< ITask > task )
{
    m_impl.dispatch_impl( thread_pool_name, request_id, std::move( task ) );
}

void
TaskDispatcherBasic::assert_execution_context( const std::string& thread_pool_name )
{
    m_impl.assert_execution_context_impl( thread_pool_name );
}

}  // namespace common
}  // namespace uni
