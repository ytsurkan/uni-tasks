#pragma once

#include "uni/common/TaskDispatcherImpl.hpp"

namespace uni
{
class TaskDispatcher
{
public:
    explicit TaskDispatcher(
        const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations
        = {{"background", 7}, {"client", 1}} );

    void start( );
    void stop( );

    /**
     * TODO: add description
     */
    template < typename F >
    void
    cancel( const std::string& thread_pool_name, RequestId request_id, F&& f )
    {
        m_impl.cancel_impl( thread_pool_name, request_id, std::forward< F >( f ) );
    }

    void cancel_pending( const std::string& thread_pool_name, RequestId request_id );
    void dispatch( const std::string& thread_pool_name, std::unique_ptr< ITask > task );
    void dispatch( const std::string& thread_pool_name,
                   RequestId request_id,
                   std::unique_ptr< ITask > task );

    // TODO: create TaskDispacherExtended, move member function inplementation to
    // TaskDispacherExtended.inl
    template < typename F, typename... Args >
    void
    dispatch( const std::string& thread_pool_name, F&& f, Args&&... args )
    {
        m_impl.dispatch_impl(
            thread_pool_name, std::forward< F >( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch( const std::string& thread_pool_name,
              Object* object,
              MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
              Args&&... args )
    {
        m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch( const std::string& thread_pool_name,
              Object* object,
              MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
              Args&&... args )
    {
        m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch( const std::string& thread_pool_name,
              const std::shared_ptr< Object >& object,
              MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
              Args&&... args )
    {
        m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch( const std::string& thread_pool_name,
              const std::shared_ptr< Object >& object,
              MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
              Args&&... args )
    {
        m_impl.dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename F, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name, F&& f, Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, std::forward< F >( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         MemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         ConstMemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         MemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         ConstMemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return m_impl.dispatch_or_execute_impl(
            thread_pool_name, object, f, std::forward< Args >( args )... );
    }

public:
    void assert_execution_context( const std::string& thread_pool_name );

private:
    TaskDispatcherImpl m_impl;
};

}  // namespace uni
