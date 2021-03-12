#pragma once

#include "uni/tasks/MemberFnTask.hpp"
#include "uni/tasks/Task.hpp"

namespace uni
{
namespace details
{
struct FunctionTag
{
};

template < typename F, typename... Args >
auto
create_task_impl( FunctionTag, F&& f, Args&&... args )
{
    return Task< std::decay_t< F >, std::decay_t< Args >... >( std::forward< F >( f ),
                                                               std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
auto
create_task_impl( Object* object, F f, Args&&... args )
{
    return MemberFnTask< Object, std::decay_t< F >, std::decay_t< Args >... >(
        object, f, std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
auto
create_task_impl( Object&& object, F f, Args&&... args )
{
    return MemberFnTask< std::decay_t< Object >, std::decay_t< F >, std::decay_t< Args >... >(
        std::forward< Object >( object ), f, std::forward< Args >( args )... );
}

template < typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr_impl( FunctionTag, F&& f, Args&&... args )
{
    return std::make_unique< Task< std::decay_t< F >, std::decay_t< Args >... > >(
        std::forward< F >( f ), std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr_impl( Object* object, F f, Args&&... args )
{
    return std::make_unique< MemberFnTask< Object, std::decay_t< F >, std::decay_t< Args >... > >(
        object, f, std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr_impl( Object&& object, F f, Args&&... args )
{
    return std::make_unique<
        MemberFnTask< std::decay_t< Object >, std::decay_t< F >, std::decay_t< Args >... > >(
        std::forward< Object >( object ), f, std::forward< Args >( args )... );
}

}  // namespace details
}  // namespace uni
