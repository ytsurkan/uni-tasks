#pragma once

#include "uni/concurrency/ThreadPool.hpp"
#include "uni/tasks/TaskComparators.hpp"
#include "uni/tasks/TaskFactory.hpp"

namespace uni
{
namespace common
{
class TaskDispatcherImpl
{
public:
    explicit TaskDispatcherImpl(
        const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations );

    void start_impl( );
    void stop_impl( );
    void assert_execution_context_impl( const std::string& thread_pool_name );
    void cancel_pending_impl( const std::string& thread_pool_name, RequestId request_id );

    /**
     * TODO: add description
     */
    template < typename F >
    void
    cancel_impl( const std::string& thread_pool_name, RequestId request_id, F&& f )
    {
        cancel_pending_impl( thread_pool_name, request_id );
        dispatch_impl( thread_pool_name, std::forward< F >( f ) );
    }

    void
    dispatch_impl( const std::string& thread_pool_name, std::unique_ptr< ITask > task )
    {
        const auto id = utils::create_request_id( );
        task->set_request_id( id );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    void
    dispatch_impl( const std::string& thread_pool_name,
                   RequestId id,
                   std::unique_ptr< ITask > task )
    {
        task->set_request_id( id );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename F, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name, F&& f, Args&&... args )
    {
        auto task = create_task_ptr( std::forward< F >( f ), std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   Object* object,
                   MemberFnPtr< Object, R, Params... > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   Object* object,
                   ConstMemberFnPtr< Object, R, Params... > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   Object* object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   Object* object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   const std::shared_ptr< Object >& object,
                   MemberFnPtr< Object, R, Params... > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   const std::shared_ptr< Object >& object,
                   ConstMemberFnPtr< Object, R, Params... > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   const std::shared_ptr< Object >& object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch_impl( const std::string& thread_pool_name,
                   const std::shared_ptr< Object >& object,
                   MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                   Args&&... args )
    {
        auto task = create_task_ptr( object, f, std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename F, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name, F&& f, Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch_impl(
                thread_pool_name, std::forward< F >( f ), std::forward< Args >( args )... );
            return false;
        }
        f( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              Object* object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                              Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              Object* object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                              Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              Object* object,
                              MemberFnPtr< Object, R, Params... > f,
                              Args&&... args )
    {
        return dispatch_or_execute_impl(
            thread_pool_name, object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              Object* object,
                              ConstMemberFnPtr< Object, R, Params... > f,
                              Args&&... args )
    {
        return dispatch_or_execute_impl(
            thread_pool_name, object, ptr2wrapper( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                              Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                              Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch_impl( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              MemberFnPtr< Object, R, Params... > f,
                              Args&&... args )
    {
        return dispatch_or_execute_impl(
            thread_pool_name, object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute_impl( const std::string& thread_pool_name,
                              const std::shared_ptr< Object >& object,
                              ConstMemberFnPtr< Object, R, Params... > f,
                              Args&&... args )
    {
        return dispatch_or_execute_impl(
            thread_pool_name, object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
    }

private:
    using ThreadPoolNameType = std::string;
    using ThreadPoolType = ThreadPool< std::unique_ptr< uni::ITask >, uni::TaskPtrCmp >;

private:
    ThreadPoolType& find_thread_pool( const std::string& thread_pool_name );

private:
    std::unordered_map< ThreadPoolNameType, ThreadPoolType > m_thread_pools{};
};

}  // namespace common
}  // namespace uni
