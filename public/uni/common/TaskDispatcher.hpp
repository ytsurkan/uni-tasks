#pragma once

#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "uni/concurrency/ThreadPool.hpp"
#include "uni/tasks/TaskComparators.hpp"
#include "uni/tasks/TaskFactory.hpp"

namespace uni
{
// TODO: Make TaskDispatcher class template

class TaskDispatcher
{
public:
    TaskDispatcher(
        const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations
        = {{"background", 7}, {"client", 1}} )
    {
        for ( const auto& thread_pool_configuration : thread_pool_configurations )
        {
            const auto thread_pool_name = std::get< 0 >( thread_pool_configuration );
            const auto threads_count = std::get< 1 >( thread_pool_configuration );

            m_thread_pools.emplace( thread_pool_name, threads_count );
        }
    }

    void
    start( )
    {
        for ( auto& element : m_thread_pools )
        {
            auto& thread_pool = element.second;
            thread_pool.start( );
        }
    }

    void
    stop( )
    {
        for ( auto& element : m_thread_pools )
        {
            auto& thread_pool = element.second;
            thread_pool.stop( );
        }
    }

    void
    cancel_pending( const std::string& thread_pool_name, RequestId request_id )
    {
        auto search = m_thread_pools.find( thread_pool_name );
        if ( search != m_thread_pools.end( ) )
        {
            auto& thread_pool = search->second;
            thread_pool.cancel_pending( request_id );
        }
    }

    /**
     * TODO: add description
     */
    template < typename F >
    void
    cancel( const std::string& thread_pool_name, RequestId request_id, F&& f )
    {
        cancel_pending( thread_pool_name, request_id );
        dispatch( thread_pool_name, std::forward< F >( f ) );
    }

    void
    dispatch( const std::string& thread_pool_name, std::unique_ptr< ITask > task )
    {
        const auto id = utils::create_request_id( );
        task->set_request_id( id );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    void
    dispatch( const std::string& thread_pool_name, RequestId id, std::unique_ptr< ITask > task )
    {
        task->set_request_id( id );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename F, typename... Args >
    void
    dispatch( const std::string& thread_pool_name, F&& f, Args&&... args )
    {
        auto task = create_task_ptr( std::forward< F >( f ), std::forward< Args >( args )... );
        task->set_request_id( utils::create_request_id( ) );
        auto& thread_pool = find_thread_pool( thread_pool_name );
        thread_pool.add_task( std::move( task ) );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    void
    dispatch( const std::string& thread_pool_name,
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
    dispatch( const std::string& thread_pool_name,
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
    dispatch( const std::string& thread_pool_name,
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
    dispatch( const std::string& thread_pool_name,
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
    dispatch_or_execute( const std::string& thread_pool_name, F&& f, Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch( thread_pool_name, std::forward< F >( f ), std::forward< Args >( args )... );
            return false;
        }
        f( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                         Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                         Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         MemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return dispatch_or_execute(
            thread_pool_name, object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         Object* object,
                         ConstMemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return dispatch_or_execute(
            thread_pool_name, object, ptr2wrapper( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                         Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                         Args&&... args )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            dispatch( thread_pool_name, object, f, std::forward< Args >( args )... );
            return false;
        }
        ( *object.*( *f ) )( std::forward< Args >( args )... );
        return true;
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         MemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return dispatch_or_execute(
            thread_pool_name, object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
    }

    template < typename Object, typename R, typename... Params, typename... Args >
    bool
    dispatch_or_execute( const std::string& thread_pool_name,
                         const std::shared_ptr< Object >& object,
                         ConstMemFnPtr< Object, R, Params... > f,
                         Args&&... args )
    {
        return dispatch_or_execute(
            thread_pool_name, object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
    }

public:
    void
    assert_execution_context( const std::string& thread_pool_name )
    {
        auto& thread_pool = find_thread_pool( thread_pool_name );
        if ( !thread_pool.on_one_of_threads( ) )
        {
            throw std::logic_error( "Assertion of running thread pool {" + thread_pool_name
                                    + "} failed!" );
        }
    }

public:
    using ThreadPoolNameType = std::string;
    using ThreadPoolType = ThreadPool< std::unique_ptr< uni::ITask >, uni::TaskPtrCmp >;

private:
    ThreadPoolType&
    find_thread_pool( const std::string& thread_pool_name )
    {
        auto search = m_thread_pools.find( thread_pool_name );
        if ( search == m_thread_pools.end( ) )
        {
            throw std::logic_error( "Unknown thread pool: " + thread_pool_name );
        }
        auto& thread_pool = search->second;
        return thread_pool;
    }

private:
    std::unordered_map< ThreadPoolNameType, ThreadPoolType > m_thread_pools{};
};

}  // namespace uni
