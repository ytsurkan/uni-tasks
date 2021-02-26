#pragma once

#include "uni/tasks/TaskBaseImpl.hpp"

namespace uni
{
template < typename F, typename... Args >
class TaskImpl : public TaskBaseImpl
{
public:
    TaskImpl( F&& f, Args&&... args )
        : m_function{std::move( f )}
        , m_args{std::forward< Args >( args )...}
    {
    }

    TaskImpl( const F& f, Args&&... args )
        : m_function{f}
        , m_args{std::forward< Args >( args )...}
    {
    }

    ~TaskImpl( ) override = default;
    TaskImpl( const TaskImpl& ) = delete;
    TaskImpl& operator=( const TaskImpl& ) = delete;
    TaskImpl( TaskImpl&& ) noexcept = default;
    TaskImpl& operator=( TaskImpl&& ) noexcept = default;

private:
    void
    call_impl( ) override
    {
        utils::apply( m_function, m_args );
    }

private:
    F m_function;
    std::tuple< std::decay_t< Args >... > m_args;
};

}  // namespace uni
