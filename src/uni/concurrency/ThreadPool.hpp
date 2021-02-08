#pragma once

#include <system_error>
#include <thread>


#include "uni/tasks/ITask.hpp"
#include "uni/concurrency/Queue.hpp"

namespace uni
{
struct ThreadPoolStoppedException : public std::exception
{
    const char*
    what( ) const noexcept override
    {
        return "QueueIsStopped";
    }
};

// TODO: Merge both ThreadPool classes to one.
template < typename TaskType, typename TaskCmp >
class ThreadPool
{
public:
    using DelayGetterPtr = TimeInterval ( * )( const TaskType& );

    explicit ThreadPool( size_t threads_count = std::thread::hardware_concurrency( ) + 1,
                         DelayGetterPtr task_delay_getter = default_task_delay_getter< TaskType > )
        : m_threads_count{threads_count}
        , m_delay_getter{task_delay_getter}
    {
    }

    ThreadPool( const ThreadPool& ) = delete;
    ThreadPool& operator=( const ThreadPool& ) = delete;

    ThreadPool( ThreadPool&& ) = delete;
    ThreadPool& operator=( ThreadPool&& ) = delete;

    ~ThreadPool( )
    {
        do_stop( );
        // TODO: Add assert and logging
        //        std::cerr << "ThreadPool::~ThreadPool(), queue empty=" << m_queue.empty( ) <<
        //        std::endl;
    }

    void
    start( )
    {
        if ( m_started )
        {
            throw std::logic_error( "ThreadPool already started!" );
        }
        m_started = true;
        do_start( );
    }

    /**
     * @brief Executes all tasks that were already added to thread pool at the moment of execution
     * of stop member function.
     */
    void
    stop( )
    {
        if ( !m_started )
        {
            throw std::logic_error( "ThreadPool already stopped!" );
        }
        m_started = false;
        do_stop( );
    }

    template < typename Task >
    void
    add_task( Task&& task )
    {
        throw_if_stopped( );
        m_queue.push( std::forward< Task >( task ) );
    }

    bool
    on_one_of_threads( )
    {
        // TODO: don't allow execution of platform specific code on other platforms.
        // TODO: replace with std::this_thread::get_id( ) == thread.get_id()
        ::pthread_t current_thread = ::pthread_self( );
        for ( auto& thread : m_threads )
        {
            if ( ::pthread_equal( current_thread, thread.native_handle( ) ) )
            {
                return true;
            }
        }
        return false;
    }

    bool
    cancel_pending( RequestId request_id )
    {
        return m_queue.remove(
            [request_id]( const TaskType& task ) { return request_id == task->request_id( ); } );
    }

private:
    void
    worker_thread( )
    {
        while ( true )
        {
            try
            {
                auto task = m_queue.wait_pop( m_delay_getter );
                if ( !task )
                {
                    return;
                }

                try
                {
                    // TODO: Add logging
                    // std::cerr << "run() id=" << task->request_id( ) << std::endl;
                    ( *task )( );
                }
                catch ( const std::exception& error )
                {
                    // TODO: log exception
                    std::cerr << "Task failed with error: " << error.what( ) << std::endl;
                }
            }
            catch ( const std::exception& )
            {
                // TODO: log exception
                return;  // std::terminate(); ???
            }
        }
    }

    void
    close( )
    {
        m_queue.close( );
    }

    void
    join( )
    {
        for ( auto& thread : m_threads )
        {
            if ( thread.joinable( ) )
            {
                try
                {
                    thread.join( );
                }
                catch ( const std::system_error& )
                {
                    // TODO: add exception handling code
                }
            }
        }
    }

    void
    do_start( )
    {
        m_threads.reserve( m_threads_count );
        for ( size_t i = 0; i < m_threads_count; ++i )
        {
            std::thread thread( [this]( ) { worker_thread( ); } );
            m_threads.push_back( std::move( thread ) );
        }
    }

    void
    do_stop( )
    {
        close( );
        join( );
    }

    void
    throw_if_stopped( )
    {
        if ( !m_started )
        {
            throw ThreadPoolStoppedException( );
        }
    }

private:
    Queue< TaskType, std::set< TaskType, TaskCmp > > m_queue;

    const size_t m_threads_count{0};
    const DelayGetterPtr m_delay_getter;
    std::atomic< bool > m_started{false};
    std::vector< std::thread > m_threads;
};

template <>
class ThreadPool< std::unique_ptr< uni::ITask >, TaskPtrCmp >
{
public:
    using TaskType = std::unique_ptr< uni::ITask >;
    using DelayGetterPtr = TimeInterval ( * )( const TaskType& );

    explicit ThreadPool( size_t threads_count = std::thread::hardware_concurrency( ) + 1 )
        : m_threads_count{threads_count}
    {
    }

    ThreadPool( const ThreadPool& ) = delete;
    ThreadPool& operator=( const ThreadPool& ) = delete;

    ThreadPool( ThreadPool&& ) = delete;
    ThreadPool& operator=( ThreadPool&& ) = delete;

    ~ThreadPool( )
    {
        do_stop( );
        // TODO: Add assert and logging
        //        std::cerr << "ThreadPool::~ThreadPool(), queue empty=" << m_queue.empty( ) <<
        //        std::endl;
    }

    void
    start( )
    {
        if ( m_started )
        {
            throw std::logic_error( "ThreadPool already started!" );
        }
        m_started = true;
        do_start( );
    }

    void
    stop( )
    {
        if ( !m_started )
        {
            throw std::logic_error( "ThreadPool already stopped!" );
        }
        m_started = false;
        do_stop( );
    }

    template < typename Task >
    void
    add_task( Task&& task )
    {
        throw_if_stopped( );
        m_queue.push( std::forward< Task >( task ) );
    }

    bool
    on_one_of_threads( )
    {
        ::pthread_t current_thread = ::pthread_self( );
        for ( auto& thread : m_threads )
        {
            if ( ::pthread_equal( current_thread, thread.native_handle( ) ) )
            {
                return true;
            }
        }
        return false;
    }

    bool
    cancel_pending( RequestId request_id )
    {
        return m_queue.remove(
            [request_id]( const TaskType& task ) { return request_id == task->request_id( ); } );
    }

private:
    void
    worker_thread( )
    {
        while ( true )
        {
            TaskType task;
            try
            {
                if ( OperationStatus::closed == m_queue.wait_pop( task, m_delay_getter ) )
                {
                    return;
                }
            }
            catch ( const std::exception& )
            {
                // TODO: log exception
                return;  // std::terminate(); ???
            }

            // TODO: Add logging
            // std::cerr << "run() id=" << task->request_id( ) << std::endl;

            try
            {
                ( *task )( );
            }
            catch ( const std::exception& error )
            {
                // TODO: log exception
                std::cerr << "Task failed with error: " << error.what( ) << std::endl;
            }
        }
    }

    void
    close( )
    {
        m_queue.close( );
    }

    void
    join( )
    {
        for ( auto& thread : m_threads )
        {
            if ( thread.joinable( ) )
            {
                try
                {
                    thread.join( );
                }
                catch ( const std::system_error& )
                {
                    // TODO: add exception handling code
                }
            }
        }
    }

    void
    do_start( )
    {
        m_threads.reserve( m_threads_count );
        for ( size_t i = 0; i < m_threads_count; ++i )
        {
            std::thread thread( [this]( ) { worker_thread( ); } );
            m_threads.push_back( std::move( thread ) );
        }
    }

    void
    do_stop( )
    {
        close( );
        join( );
    }

    void
    throw_if_stopped( )
    {
        if ( !m_started )
        {
            throw ThreadPoolStoppedException( );
        }
    }

private:
    Queue< TaskType, std::set< TaskType, TaskPtrCmp > > m_queue;

    const size_t m_threads_count{0};
    const DelayGetterPtr m_delay_getter{
        []( const TaskType& task ) -> TimeInterval { return task->delay( ); }};
    std::atomic< bool > m_started{false};
    std::vector< std::thread > m_threads;
};

}  // namespace uni
