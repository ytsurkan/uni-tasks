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
    return Task< std::decay_t< F >, Args... >( std::forward< F >( f ),
                                               std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
auto
create_task_impl( Object* object, F f, Args&&... args )
{
    return MemberFnTask< Object, std::decay_t< F >, Args... >(
        object, f, std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
auto
create_task_impl( Object&& object, F f, Args&&... args )
{
    return MemberFnTask< std::decay_t< Object >, std::decay_t< F >, Args... >(
        std::forward< Object >( object ), f, std::forward< Args >( args )... );
}

template < typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr_impl( FunctionTag, F&& f, Args&&... args )
{
    return std::make_unique< Task< std::decay_t< F >, Args... > >(
        std::forward< F >( f ), std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr_impl( Object* object, F f, Args&&... args )
{
    return std::make_unique< MemberFnTask< Object, std::decay_t< F >, Args... > >(
        object, f, std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr_impl( Object&& object, F f, Args&&... args )
{
    return std::make_unique< MemberFnTask< std::decay_t< Object >, std::decay_t< F >, Args... > >(
        std::forward< Object >( object ), f, std::forward< Args >( args )... );
}

}  // namespace details

template < typename F, typename... Args >
auto
create_task( F&& f, Args&&... args )
{
    return details::create_task_impl(
        details::FunctionTag( ), std::forward< F >( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, MemFnPtrWrapper< R ( Object::* )( Params... ) > f, Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object,
             MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, MemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, ConstMemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemFnPtr< Object, R, Params... > f,
             Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             ConstMemFnPtr< Object, R, Params... > f,
             Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( F&& f, Args&&... args )
{
    return details::create_task_ptr_impl(
        details::FunctionTag( ), std::forward< F >( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, MemFnPtrWrapper< R ( Object::* )( Params... ) > f, Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object,
                 MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, MemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, ConstMemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemFnPtr< Object, R, Params... > f,
                 Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 ConstMemFnPtr< Object, R, Params... > f,
                 Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

}  // namespace uni
