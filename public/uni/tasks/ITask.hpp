#pragma once

#include <future>
#include "uni/Types.hpp"

namespace uni
{
class ITask
{
public:
    ITask( ) = default;
    virtual ~ITask( ) = default;

    ITask( const ITask& ) = default;
    ITask& operator=( const ITask& ) = default;
    ITask( ITask&& ) noexcept = default;
    ITask& operator=( ITask&& ) noexcept = default;

    void
    operator( )( )
    {
        run_impl( );
    }

    void
    run( )
    {
        run_impl( );
    }

    RequestId
    request_id( ) const
    {
        return request_id_impl( );
    }

    void
    set_request_id( RequestId request_id )
    {
        set_request_id_impl( request_id );
    }

    /**
     * @brief Returns the unique identifier of task.
     * @return sequence id
     */
    SequenceId
    sequence_id( ) const
    {
        return sequence_id_impl( );
    }

    /**
     * @brief Sets the due time as current time (epoch time) + delay time in ms.
     */
    void
    set_due_time( TimeInterval delay_ms )
    {
        set_due_time_impl( delay_ms );
    }

    /**
     * @brief Returns the due time (epoch time) in ms.
     * @return due time in ms.
     */
    TimeInterval
    due_time( ) const
    {
        return due_time_impl( );
    }

    /**
     * @brief Returns the delay time in ms from the current time.
     * @note This function is nondeterministic, if we call it two times it may return different
     * result each time.
     * @return delay time in ms.
     */
    TimeInterval
    delay( ) const
    {
        return delay_impl( );
    }

    /**
     * @brief Returns a std::future associated with the promised result of task, that allows to wait
     * for task completion or get the exception that occured on task execution. The get_future()
     * should be called before the task execution, otherwise returned std::future will not be
     * associated with the promised result of task.
     * @return returns a std::future associated with the promised result of task.
     */
    std::future< void >
    get_future( )
    {
        return get_future_impl( );
    }

protected:
    virtual void run_impl( ) = 0;
    virtual RequestId request_id_impl( ) const = 0;
    virtual void set_request_id_impl( RequestId request_id ) = 0;
    virtual SequenceId sequence_id_impl( ) const = 0;
    virtual void set_due_time_impl( TimeInterval delay ) = 0;
    virtual TimeInterval due_time_impl( ) const = 0;
    virtual TimeInterval delay_impl( ) const = 0;
    virtual std::future< void > get_future_impl( ) = 0;
};

}  // namespace uni
