#pragma once

#include "uni/tasks/details/TaskBaseImpl.hpp"

namespace uni
{
template < typename F, typename... Args >
class TaskImpl : public TaskBaseImpl
{
public:
    template < typename TF, typename... TArgs >
    TaskImpl( TF&& f, TArgs&&... args )
        : m_function( std::forward< TF >( f ) )
        , m_args( std::forward< TArgs >( args )... )
    {
    }

    ~TaskImpl( ) override = default;
    TaskImpl( const TaskImpl& ) = delete;
    TaskImpl& operator=( const TaskImpl& ) = delete;
    TaskImpl( TaskImpl&& ) = default;
    TaskImpl& operator=( TaskImpl&& ) = default;

private:
    void
    call_impl( ) override
    {
        utils::apply( m_function, m_args );
    }

private:
    F m_function;
    std::tuple< Args... > m_args;
};

}  // namespace uni
