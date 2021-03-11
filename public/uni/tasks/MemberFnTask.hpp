#pragma once

#include "uni/tasks/ITask.hpp"
#include "uni/tasks/MemberFnTaskImpl.hpp"

namespace uni
{
template < typename Object, typename F, typename... Args >
class MemberFnTask : public ITask
{
public:
    template < typename... TArgs >
    MemberFnTask( Object* object, F f, TArgs&&... args )
        : m_impl{object, f, std::forward< TArgs >( args )...}
    {
    }

    ~MemberFnTask( ) override = default;
    MemberFnTask( const MemberFnTask& ) = delete;
    MemberFnTask& operator=( const MemberFnTask& ) = delete;
    MemberFnTask( MemberFnTask&& ) noexcept = default;
    MemberFnTask& operator=( MemberFnTask&& ) noexcept = default;

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
    MemberFnTaskImpl< Object, F, Args... > m_impl;
};

template < typename Object, typename F, typename... Args >
class MemberFnTask< std::shared_ptr< Object >, F, Args... > : public ITask
{
public:
    template < typename TObject, typename... TArgs >
    MemberFnTask( TObject&& object, F f, TArgs&&... args )
        : m_impl{std::forward< TObject >( object ), f, std::forward< TArgs >( args )...}
    {
    }

    ~MemberFnTask( ) override = default;
    MemberFnTask( const MemberFnTask& ) = delete;
    MemberFnTask& operator=( const MemberFnTask& ) = delete;
    MemberFnTask( MemberFnTask&& ) noexcept = default;
    MemberFnTask& operator=( MemberFnTask&& ) noexcept = default;

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
    MemberFnTaskImpl< std::shared_ptr< Object >, F, Args... > m_impl;
};

}  // namespace uni
