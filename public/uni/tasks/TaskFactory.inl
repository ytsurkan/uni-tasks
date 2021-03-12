#pragma once

#include "uni/tasks/TaskFactoryImpl.hpp"

namespace uni
{
template < typename F, typename... Args >
auto
create_task( F&& f, Args&&... args )
{
    return details::create_task_impl(
        details::FunctionTag( ), std::forward< F >( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, MemberFnPtrWrapper< R ( Object::* )( Params... ) > f, Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object,
             MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, MemberFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, ConstMemberFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemberFnPtrWrapper< R ( Object::* )( Params... ) > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemberFnPtr< Object, R, Params... > f,
             Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             ConstMemberFnPtr< Object, R, Params... > f,
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
create_task_ptr( Object* object,
                 MemberFnPtrWrapper< R ( Object::* )( Params... ) > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object,
                 MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, ConstMemberFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemberFnPtrWrapper< R ( Object::* )( Params... ) > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemberFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemberFnPtr< Object, R, Params... > f,
                 Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 ConstMemberFnPtr< Object, R, Params... > f,
                 Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

}  // namespace uni
