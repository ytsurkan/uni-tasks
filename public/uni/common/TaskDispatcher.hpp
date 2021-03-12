#pragma once

#include "uni/common/TaskDispatcherImpl.hpp"

namespace uni
{
namespace common
{
class TaskDispatcher
{
public:
    explicit TaskDispatcher(
        const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations
        = {{"background", 7}, {"client", 1}} );

    void start( );
    void stop( );

    void assert_execution_context( const std::string& thread_pool_name );

    void dispatch( const std::string& thread_pool_name, std::unique_ptr< ITask > task );
    void dispatch( const std::string& thread_pool_name,
                   RequestId request_id,
                   std::unique_ptr< ITask > task );
    void cancel_pending( const std::string& thread_pool_name, RequestId request_id );

    /**
     * TODO: add description
     */
    template < typename F >
    void cancel( const std::string& thread_pool_name, RequestId request_id, F&& f );

    template < typename F, typename... Args >
    void dispatch( const std::string& thread_pool_name, F&& f, Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    void dispatch( const std::string& thread_pool_name,
                   Object* object,
                   MemberFnPtr< Object, R, Params... > f,
                   Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    void dispatch( const std::string& thread_pool_name,
                   Object* object,
                   ConstMemberFnPtr< Object, R, Params... > f,
                   Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    void dispatch( const std::string& thread_pool_name,
                   Object* object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                   Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    void dispatch( const std::string& thread_pool_name,
                   Object* object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                   Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    void dispatch( const std::string& thread_pool_name,
                   const std::shared_ptr< Object >& object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                   Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    void dispatch( const std::string& thread_pool_name,
                   const std::shared_ptr< Object >& object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                   Args&&... args );

    template < typename F, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name, F&& f, Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              Object* object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                              Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              Object* object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                              Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              Object* object,
                              MemberFnPtr< Object, R, Params... > f,
                              Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              Object* object,
                              ConstMemberFnPtr< Object, R, Params... > f,
                              Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                              Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                              Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              MemberFnPtr< Object, R, Params... > f,
                              Args&&... args );

    template < typename Object, typename R, typename... Params, typename... Args >
    bool dispatch_or_execute( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              ConstMemberFnPtr< Object, R, Params... > f,
                              Args&&... args );

private:
    TaskDispatcherImpl m_impl;
};

}  // namespace common
}  // namespace uni

#include "uni/common/TaskDispatcher.inl"
