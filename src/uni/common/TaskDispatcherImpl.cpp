#include "uni/common/details/TaskDispatcherImpl.hpp"

namespace uni
{
namespace common
{
TaskDispatcherImpl::TaskDispatcherImpl(
    const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations )
{
    for ( const auto& thread_pool_configuration : thread_pool_configurations )
    {
        const auto thread_pool_name = std::get< 0 >( thread_pool_configuration );
        const auto threads_count = std::get< 1 >( thread_pool_configuration );

        m_thread_pools.emplace( thread_pool_name, threads_count );
    }
}

void
TaskDispatcherImpl::start_impl( )
{
    for ( auto& element : m_thread_pools )
    {
        auto& thread_pool = element.second;
        thread_pool.start( );
    }
}

void
TaskDispatcherImpl::stop_impl( )
{
    for ( auto& element : m_thread_pools )
    {
        auto& thread_pool = element.second;
        thread_pool.stop( );
    }
}

void
TaskDispatcherImpl::cancel_pending_impl( const std::string& thread_pool_name, RequestId request_id )
{
    auto search = m_thread_pools.find( thread_pool_name );
    if ( search != m_thread_pools.end( ) )
    {
        auto& thread_pool = search->second;
        thread_pool.cancel_pending( request_id );
    }
}

void
TaskDispatcherImpl::assert_execution_context_impl( const std::string& thread_pool_name )
{
    auto& thread_pool = find_thread_pool( thread_pool_name );
    if ( !thread_pool.on_one_of_threads( ) )
    {
        throw std::logic_error( "Assertion of running thread pool {" + thread_pool_name
                                + "} failed!" );
    }
}

TaskDispatcherImpl::ThreadPoolType&
TaskDispatcherImpl::find_thread_pool( const std::string& thread_pool_name )
{
    auto search = m_thread_pools.find( thread_pool_name );
    if ( search == m_thread_pools.end( ) )
    {
        throw std::logic_error( "Unknown thread pool: " + thread_pool_name );
    }
    auto& thread_pool = search->second;
    return thread_pool;
}

}  // namespace common
}  // namespace uni
