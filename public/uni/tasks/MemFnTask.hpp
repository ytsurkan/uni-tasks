#pragma once

#include "uni/tasks/ITask.hpp"
#include "uni/tasks/MemberFnTaskImpl.hpp"

namespace uni
{
template < typename Object, typename F, typename... Args >
class MemberFnTask : public ITask
{
public:
    MemberFnTask( Object* object, F f, Args&&... args )
        : m_impl{object, f, std::forward< Args >( args )...}
    {
    }

    ~MemberFnTask( ) override = default;
    MemberFnTask( const MemberFnTask& ) = delete;
    MemberFnTask& operator=( const MemberFnTask& ) = delete;
    MemberFnTask( MemberFnTask&& ) noexcept = default;
    MemberFnTask& operator=( MemberFnTask&& ) noexcept = default;

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
    MemberFnTaskImpl< Object, F, Args... > m_impl;
};

template < typename Object, typename F, typename... Args >
class MemberFnTask< std::shared_ptr< Object >, F, Args... > : public ITask
{
public:
    MemberFnTask( std::shared_ptr< Object >&& object, F f, Args&&... args )
        : m_impl{std::move( object ), f, std::forward< Args >( args )...}
    {
    }

    MemberFnTask( const std::shared_ptr< Object >& object, F f, Args&&... args )
        : m_impl{object, f, std::forward< Args >( args )...}
    {
    }

    ~MemberFnTask( ) override = default;
    MemberFnTask( const MemberFnTask& ) = delete;
    MemberFnTask& operator=( const MemberFnTask& ) = delete;
    MemberFnTask( MemberFnTask&& ) noexcept = default;
    MemberFnTask& operator=( MemberFnTask&& ) noexcept = default;

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
    MemberFnTaskImpl< std::shared_ptr< Object >, F, Args... > m_impl;
};

}  // namespace uni
