#pragma once

#include "uni/tasks/TaskBaseImpl.hpp"

namespace uni
{
template < typename Object, typename F, typename... Args >
class MemberFnTaskImpl : public TaskBaseImpl
{
public:
    MemberFnTaskImpl( Object* object, F f, Args&&... args )
        : m_object{object}
        , m_function{f}
        , m_args{std::forward< Args >( args )...}
    {
    }

    ~MemberFnTaskImpl( ) override = default;
    MemberFnTaskImpl( const MemberFnTaskImpl& ) = delete;
    MemberFnTaskImpl& operator=( const MemberFnTaskImpl& ) = delete;
    MemberFnTaskImpl( MemberFnTaskImpl&& ) noexcept = default;
    MemberFnTaskImpl& operator=( MemberFnTaskImpl&& ) noexcept = default;

private:
    void
    call_impl( ) override
    {
        call_impl( m_args, std::index_sequence_for< Args... >( ) );
    }

private:
    template < typename Tuple, size_t... Indices >
    void
    call_impl( Tuple&& tuple, std::index_sequence< Indices... > )
    {
        utils::invoke( std::forward< F >( m_function ),
                       *m_object,
                       std::forward< Args >( std::get< Indices >( tuple ) )... );
    }

private:
    Object* m_object{nullptr};
    F m_function;
    std::tuple< std::decay_t< Args >... > m_args;
};

template < typename Object, typename F, typename... Args >
class MemberFnTaskImpl< std::shared_ptr< Object >, F, Args... > : public TaskBaseImpl
{
public:
    MemberFnTaskImpl( std::shared_ptr< Object >&& object, F f, Args&&... args )
        : m_object{std::move( object )}
        , m_function{f}
        , m_args{std::forward< Args >( args )...}
    {
    }

    MemberFnTaskImpl( const std::shared_ptr< Object >& object, F f, Args&&... args )
        : m_object{object}
        , m_function{f}
        , m_args{std::forward< Args >( args )...}
    {
    }

    ~MemberFnTaskImpl( ) override = default;
    MemberFnTaskImpl( const MemberFnTaskImpl& ) = delete;
    MemberFnTaskImpl& operator=( const MemberFnTaskImpl& ) = delete;
    MemberFnTaskImpl( MemberFnTaskImpl&& ) noexcept = default;
    MemberFnTaskImpl& operator=( MemberFnTaskImpl&& ) noexcept = default;

private:
    void
    call_impl( ) override
    {
        call_impl( m_args, std::index_sequence_for< Args... >( ) );
    }

private:
    template < typename Tuple, size_t... Indices >
    void
    call_impl( Tuple&& tuple, std::index_sequence< Indices... > )
    {
        utils::invoke( std::forward< F >( m_function ),
                       *m_object,
                       std::forward< Args >( std::get< Indices >( tuple ) )... );
    }

private:
    std::shared_ptr< Object > m_object;
    F m_function;
    std::tuple< std::decay_t< Args >... > m_args;
};

}  // namespace uni
