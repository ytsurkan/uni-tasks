#pragma once

#include "uni/concurrency/ThreadPoolImpl.hpp"

namespace uni
{
template < typename TaskType, typename TaskCmp >
class ThreadPool
{
    using ImplType = ThreadPoolImpl< TaskType, TaskCmp >;

public:
    explicit ThreadPool( size_t threads_count = std::thread::hardware_concurrency( ) )
        : m_impl{threads_count}
    {
    }

    /**
     * @brief Start thread pool execution.
     */
    void
    start( )
    {
        m_impl.start_impl( );
    }

    /**
     * @brief Stop thread pool execution. Executes all tasks that were already added to thread pool
     * at the moment of execution of stop_impl member function.
     */
    void
    stop( )
    {
        m_impl.stop_impl( );
    }

    /**
     * @brief Add task to the thread pool to be executed. Throw exception if thread pool is stopped.
     */
    template < typename Task >
    void
    add_task( Task&& task )
    {
        m_impl.add_task_impl( std::forward< Task >( task ) );
    }

    /**
     * @brief Checks if on_one_of_threads is executing in the thread context of one of the threads
     * of thread pool.
     * @return true if on_one_of_threads is called in the thread context of one of the threads of
     * thread pool, otherwise false.
     */
    bool
    on_one_of_threads( ) const
    {
        return m_impl.on_one_of_threads_impl( );
    }

    /**
     * @brief Tries to remove pending task from queue.
     * @param request_id request Id of task to be removed.
     * @return Return true if task with request_id was found and removed from queue, otherwise
     * false.
     */
    bool
    cancel_pending( RequestId request_id )
    {
        return m_impl.cancel_pending_impl( request_id );
    }

private:
    ImplType m_impl;
};

}  // namespace uni
