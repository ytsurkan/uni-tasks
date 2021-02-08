#pragma once

#include "uni/utils/Utils.hpp"

#include "uni/tasks/MemFnTask.hpp"
#include "uni/tasks/Task.hpp"

namespace uni {

namespace details {

struct FunctionTag
{};

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
    return MemFnTask< Object, std::decay_t< F >, Args... >(
        object, f, std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
auto
create_task_impl( Object&& object, F f, Args&&... args )
{
    return MemFnTask< std::decay_t< Object >, std::decay_t< F >, Args... >(
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
    return std::make_unique< MemFnTask< Object, std::decay_t< F >, Args... > >(
        object, f, std::forward< Args >( args )... );
}

template < typename Object, typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr_impl( Object&& object, F f, Args&&... args )
{
    return std::make_unique< MemFnTask< std::decay_t< Object >, std::decay_t< F >, Args... > >(
        std::forward< Object >( object ), f, std::forward< Args >( args )... );
}

} // namespace details

/**
 * This factory function creates task object from function or lambda expression.
 * @param f Function of lamda expression.
 * @param args... Arguments of function or lambda.
 * @return Task object.
 */
template < typename F, typename... Args >
auto
create_task( F&& f, Args&&... args )
{
    return details::create_task_impl(
        details::FunctionTag( ), std::forward< F >( f ), std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Pointer to object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, MemFnPtrWrapper< R ( Object::* )( Params... ) > f, Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Pointer to object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object,
             MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, MemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( Object* object, ConstMemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to const member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
             Args&&... args )
{
    return details::create_task_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             MemFnPtr< Object, R, Params... > f,
             Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto
create_task( const std::shared_ptr< Object >& object,
             ConstMemFnPtr< Object, R, Params... > f,
             Args&&... args )
{
    return create_task( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

/**
 * This factory function creates unique pointer to task object. Task object can be created from
 * functions and lambda expressions.
 * @param f Function of lamda expression.
 * @param args... Arguments of function or lambda.
 * @return Unique pointer to task object.
 */
template < typename F, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( F&& f, Args&&... args )
{
    return details::create_task_ptr_impl(
        details::FunctionTag( ), std::forward< F >( f ), std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Pointer to object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, MemFnPtrWrapper< R ( Object::* )( Params... ) > f, Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Pointer to object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to const member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object,
                 MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, MemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, ConstMemFnPtr< Object, R, Params... > f, Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function allows to specify explicitly the prototype of function to be called.
 * For overloaded functions it's not clear which function should be taken, therefore
 * the function prototype of overloaded function should be explicitly specified.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Instance of MemFnPtrWrapper that takes a pointer to const member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                 Args&&... args )
{
    return details::create_task_ptr_impl( object, *f, std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 MemFnPtr< Object, R, Params... > f,
                 Args&&... args )
{
    return create_task_ptr( object, ptr2wrapper( f ), std::forward< Args >( args )... );
}

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( const std::shared_ptr< Object >& object,
                 ConstMemFnPtr< Object, R, Params... > f,
                 Args&&... args )
{
    return create_task_ptr( object, utils::ptr2wrapper( f ), std::forward< Args >( args )... );
}

} // namespace uni
