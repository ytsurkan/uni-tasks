#pragma once

#include "uni/concurrency/ThreadPoolImpl.hpp"
#include "uni/tasks/ITask.hpp"
#include "uni/tasks/TaskComparators.hpp"

namespace uni
{
// TODO: Merge both ThreadPool classes to one.
template < typename TaskType, typename TaskCmp >
class ThreadPool : private ThreadPoolImpl< TaskType, TaskCmp >
{
    using ImplType = ThreadPoolImpl< TaskType, TaskCmp >;

public:
    explicit ThreadPool( size_t threads_count = std::thread::hardware_concurrency( ) + 1 )
        : ImplType{threads_count}
    {
    }

    void
    start( )
    {
        this->start_impl( );
    }

    /**
     * @brief Executes all tasks that were already added to thread pool at the moment of execution
     * of stop_impl member function.
     */
    void
    stop( )
    {
        this->stop_impl( );
    }

    template < typename Task >
    void
    add_task( Task&& task )
    {
        this->add_task_impl( std::forward< Task >( task ) );
    }

    bool
    on_one_of_threads( ) const
    {
        return this->on_one_of_threads_impl( );
    }

    bool
    cancel_pending( RequestId request_id )
    {
        return this->cancel_pending_impl( request_id );
    }

private:
    void
    worker_thread_impl( ) override
    {
        const auto get_delay{[]( const TaskType& task ) -> TimeInterval { return task.delay( ); }};
        while ( true )
        {
            try
            {
                auto task = this->m_queue.wait_pop( get_delay );
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
};

template <>
class ThreadPool< std::unique_ptr< uni::ITask >, TaskPtrCmp >
    : private ThreadPoolImpl< std::unique_ptr< uni::ITask >, TaskPtrCmp >
{
    using ImplType = ThreadPoolImpl< std::unique_ptr< uni::ITask >, TaskPtrCmp >;

public:
    explicit ThreadPool( size_t threads_count = std::thread::hardware_concurrency( ) + 1 )
        : ImplType{threads_count}
    {
    }

    /**
     * @brief Start thread pool.
     */
    void
    start( )
    {
        this->start_impl( );
    }

    /**
     * @brief Stop thread pool.
     * @note Executes all tasks that were already added to thread pool at the moment of execution
     * of stop_impl member function.
     */
    void
    stop( )
    {
        this->stop_impl( );
    }

    template < typename Task >
    void
    add_task( Task&& task )
    {
        this->add_task_impl( std::forward< Task >( task ) );
    }

    bool
    on_one_of_threads( ) const
    {
        return this->on_one_of_threads_impl( );
    }

    bool
    cancel_pending( RequestId request_id )
    {
        return this->cancel_pending_impl( request_id );
    }

private:
    void
    worker_thread_impl( ) override
    {
        using TaskType = std::unique_ptr< uni::ITask >;

        const auto get_delay{[]( const TaskType& task ) -> TimeInterval { return task->delay( ); }};
        while ( true )
        {
            TaskType task;
            try
            {
                if ( OperationStatus::closed == m_queue.wait_pop( task, get_delay ) )
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
};

}  // namespace uni
