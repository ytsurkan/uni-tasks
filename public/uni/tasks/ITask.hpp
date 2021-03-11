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

    ITask( const ITask& ) = delete;
    ITask& operator=( const ITask& ) = delete;
    ITask( ITask&& ) noexcept = default;
    ITask& operator=( ITask&& ) noexcept = default;

    /**
     * @brief Runs the function or member function associated with this task.
     */
    virtual void run( ) = 0;

    /**
     * @brief Gets the request unique identifier.
     * @return request identifier.
     */
    virtual RequestId request_id( ) const = 0;

    /**
     * @brief Sets the request identifier.
     * @param request_id request identifier.
     */
    virtual void set_request_id( RequestId request_id ) = 0;

    /**
     * @brief Returns the unique identifier of task.
     * @return sequence id
     */
    virtual SequenceId sequence_id( ) const = 0;

    /**
     * @brief Sets the due time as current time (epoch time) + delay time in ms.
     */
    virtual void set_due_time( TimeInterval delay ) = 0;

    /**
     * @brief Returns the due time (epoch time) in ms.
     * @return due time in ms.
     */
    virtual TimeInterval due_time( ) const = 0;

    /**
     * @brief Returns the delay time in ms from the current time.
     * @note This function is nondeterministic, if we call it two times it may return different
     * result each time.
     * @return delay time in ms.
     */
    virtual TimeInterval delay( ) const = 0;

    /**
     * @brief Returns a std::future associated with the promised result of task, that allows
     * to wait  for task completion or get the exception that occured on task execution.
     * The get_future() should be called before the task execution, otherwise returned std::future
     * will not be associated with the promised result of task.
     * @return returns a std::future associated with the promised result of task.
     */
    virtual std::future< void > get_future( ) = 0;
};

}  // namespace uni
