#pragma once

#include <atomic>
#include <thread>

#include "uni/Types.hpp"
#include "uni/concurrency/Queue.hpp"
#include "uni/tasks/TaskUtils.hpp"
#include "uni/utils/Utils.hpp"

// TODO: remove iostream
#include <iostream>

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

template < typename TaskType, typename TaskCmp >
class ThreadPoolImpl
{
public:
    explicit ThreadPoolImpl( size_t threads_count )
        : m_threads_count{threads_count}
    {
    }

    ThreadPoolImpl( const ThreadPoolImpl& ) = delete;
    ThreadPoolImpl& operator=( const ThreadPoolImpl& ) = delete;

    ThreadPoolImpl( ThreadPoolImpl&& ) = delete;
    ThreadPoolImpl& operator=( ThreadPoolImpl&& ) = delete;

    virtual ~ThreadPoolImpl( )
    {
        do_stop( );
        // TODO: Add assert and logging
        //        std::cerr << "ThreadPool::~ThreadPool(), queue empty=" << m_queue.empty( ) <<
        //        std::endl;
    }

    void
    start_impl( )
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
     * of stop_impl member function.
     */
    void
    stop_impl( )
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
    add_task_impl( Task&& task )
    {
        throw_if_stopped( );
        m_queue.push( std::forward< Task >( task ) );
    }

    bool
    on_one_of_threads_impl( ) const
    {
        for ( const auto& thread : m_threads )
        {
            if ( std::this_thread::get_id( ) == thread.get_id( ) )
            {
                return true;
            }
        }
        return false;
    }

    bool
    cancel_pending_impl( RequestId request_id )
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
                auto task_optional = this->m_queue.wait_pop( &tasks::get_task_delay< TaskType > );
                if ( !task_optional )
                {
                    return;
                }

                try
                {
                    // TODO: Add logging
                    // std::cerr << "run() id=" << task_optional->request_id( ) << std::endl;
                    auto& task = utils::get_value( task_optional );
                    task( );
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
    Queue< TaskType, std::set< TaskType, TaskCmp > > m_queue{};
    const size_t m_threads_count{0};
    std::atomic< bool > m_started{false};
    std::vector< std::thread > m_threads{};
};

}  // namespace uni
