#pragma once

#include "uni/tasks/ITask.hpp"
#include "uni/tasks/TaskImpl.hpp"

namespace uni
{
template < typename F, typename... Args >
class Task : public ITask
{
public:
    Task( F&& f, Args&&... args )
        : m_impl{std::forward< F >( f ), std::forward< Args >( args )...}
    {
    }

    Task( const F& f, Args&&... args )
        : m_impl{f, std::forward< Args >( args )...}
    {
    }

    ~Task( ) override = default;
    Task( const Task& ) = delete;
    Task& operator=( const Task& ) = delete;
    Task( Task&& ) noexcept = default;
    Task& operator=( Task&& ) noexcept = default;

protected:
    /// ITask interface implementation
    void
    run_impl( ) override
    {
        m_impl.run_impl( );
    }

    RequestId
    request_id_impl( ) const override
    {
        return m_impl.request_id_impl( );
    }

    void
    set_request_id_impl( RequestId request_id ) override
    {
        m_impl.set_request_id_impl( request_id );
    }

    SequenceId
    sequence_id_impl( ) const override
    {
        return m_impl.sequence_id_impl( );
    }

    void
    set_due_time_impl( TimeInterval delay ) override
    {
        m_impl.set_due_time_impl( delay );
    }

    TimeInterval
    due_time_impl( ) const override
    {
        return m_impl.due_time_impl( );
    }

    TimeInterval
    delay_impl( ) const override
    {
        return m_impl.delay_impl( );
    }

    std::future< void >
    get_future_impl( ) override
    {
        return m_impl.get_future_impl( );
    }

private:
    TaskImpl< F, Args... > m_impl;
};

}  // namespace uni
