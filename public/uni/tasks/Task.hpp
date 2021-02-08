#pragma once

#include "uni/tasks/TaskBase.hpp"
#include "uni/utils/Utils.hpp"

namespace uni
{

template < typename F, typename... Args >
class Task : public TaskBase
{
public:
    Task( F&& f, Args&&... args )
        : m_function{std::move( f )}
        , m_args{std::forward< Args >( args )...}
    {
    }

    Task( const F& f, Args&&... args )
        : m_function{f}
        , m_args{std::forward< Args >( args )...}
    {
    }

    ~Task( ) override = default;
    Task( const Task& ) = delete;
    Task& operator=( const Task& ) = delete;
    Task( Task&& ) noexcept = default;
    Task& operator=( Task&& ) noexcept = default;

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
