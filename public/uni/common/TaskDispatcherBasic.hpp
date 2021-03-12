#pragma once

#include "uni/common/TaskDispatcherImpl.hpp"

namespace uni
{
namespace common
{
class TaskDispatcherBasic
{
public:
    explicit TaskDispatcherBasic(
        const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations
        = {{"background", 7}, {"client", 1}} );

    void start( );
    void stop( );
    void assert_execution_context( const std::string& thread_pool_name );
    void cancel_pending( const std::string& thread_pool_name, RequestId request_id );

    /**
     * TODO: add description
     */
    void cancel( const std::string& thread_pool_name,
                 RequestId request_id,
                 std::function< void( ) > f );

    void dispatch( const std::string& thread_pool_name,
                   RequestId request_id,
                   std::unique_ptr< ITask > task );

    template < typename Object, typename R, typename... Params, typename... Args >
    void dispatch( const std::string& thread_pool_name,
                   Object* object,
                   MemberFnPtr< Object, R, Params... > f,
                   Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              Object* object,
                              MemberFnPtr< Object, R, Params... > f,
                              Args&&... args );

private:
    TaskDispatcherImpl m_impl;
};

}  // namespace common
}  // namespace uni

#include "uni/common/TaskDispatcherBasic.inl"
