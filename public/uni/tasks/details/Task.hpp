#pragma once

#include "uni/tasks/ITask.hpp"
#include "uni/tasks/details/TaskImpl.hpp"

namespace uni
{
template < typename F, typename... Args >
class Task : public ITask
{
public:
    /**
     * @brief
     * std::decay_t models the type conversions applied to function arguments when passed by value.
     * std::is_same_v<C, Task> is true for case "auto task2 = task;"
     * std::enable_if_t disables calling of this constructor instead of copy constructor for
     * non-const objects.
     */
    template < typename TF,
               typename C = std::decay_t< TF >,
               typename = typename std::enable_if_t< !( std::is_same_v< C, Task > ) >,
               typename... TArgs >
    Task( TF&& f, TArgs&&... args )
        : m_impl( std::forward< TF >( f ), std::forward< TArgs >( args )... )
    {
    }

    ~Task( ) override = default;
    Task( const Task& ) = delete;
    Task& operator=( const Task& ) = delete;
    Task( Task&& ) noexcept = default;
    Task& operator=( Task&& ) noexcept = default;

private:
    /// ITask interface implementation
    void
    run( ) override
    {
        m_impl.run_impl( );
    }

    RequestId
    request_id( ) const override
    {
        return m_impl.request_id_impl( );
    }

    void
    set_request_id( RequestId request_id ) override
    {
        m_impl.set_request_id_impl( request_id );
    }

    SequenceId
    sequence_id( ) const override
    {
        return m_impl.sequence_id_impl( );
    }

    void
    set_due_time( TimeInterval delay ) override
    {
        m_impl.set_due_time_impl( delay );
    }

    TimeInterval
    due_time( ) const override
    {
        return m_impl.due_time_impl( );
    }

    TimeInterval
    delay( ) const override
    {
        return m_impl.delay_impl( );
    }

    std::future< void >
    get_future( ) override
    {
        return m_impl.get_future_impl( );
    }

private:
    TaskImpl< F, Args... > m_impl;
};

}  // namespace uni
