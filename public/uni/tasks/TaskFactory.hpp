#pragma once

#include "uni/tasks/TaskFactory.inl"
#include "uni/tasks/TaskFactoryBasic.hpp"

namespace uni
{
/**
 * This factory function creates task object from function or lambda expression.
 * @param f Function of lamda expression.
 * @param args... Arguments of function or lambda.
 * @return Task object.
 */
template < typename F, typename... Args >
auto create_task( F&& f, Args&&... args );

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
auto create_task( Object* object,
                  MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                  Args&&... args );

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
auto create_task( Object* object,
                  MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                  Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto create_task( Object* object, MemberFnPtr< Object, R, Params... > f, Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto create_task( Object* object, ConstMemberFnPtr< Object, R, Params... > f, Args&&... args );

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
auto create_task( const std::shared_ptr< Object >& object,
                  MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                  Args&&... args );

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
auto create_task( const std::shared_ptr< Object >& object,
                  MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                  Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto create_task( const std::shared_ptr< Object >& object,
                  MemberFnPtr< Object, R, Params... > f,
                  Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
auto create_task( const std::shared_ptr< Object >& object,
                  ConstMemberFnPtr< Object, R, Params... > f,
                  Args&&... args );

/**
 * This factory function creates unique pointer to task object. Task object can be created from
 * functions and lambda expressions.
 * @param f Function of lamda expression.
 * @param args... Arguments of function or lambda.
 * @return Unique pointer to task object.
 */
template < typename F, typename... Args >
std::unique_ptr< ITask > create_task_ptr( F&& f, Args&&... args );

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
std::unique_ptr< ITask > create_task_ptr( Object* object,
                                          MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                                          Args&&... args );

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
std::unique_ptr< ITask > create_task_ptr( Object* object,
                                          MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                                          Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask > create_task_ptr( Object* object,
                                          MemberFnPtr< Object, R, Params... > f,
                                          Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Pointer to object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask > create_task_ptr( Object* object,
                                          ConstMemberFnPtr< Object, R, Params... > f,
                                          Args&&... args );

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
std::unique_ptr< ITask > create_task_ptr( const std::shared_ptr< Object >& object,
                                          MemFnPtrWrapper< R ( Object::* )( Params... ) > f,
                                          Args&&... args );

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
std::unique_ptr< ITask > create_task_ptr( const std::shared_ptr< Object >& object,
                                          MemFnPtrWrapper< R ( Object::* )( Params... ) const > f,
                                          Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask > create_task_ptr( const std::shared_ptr< Object >& object,
                                          MemberFnPtr< Object, R, Params... > f,
                                          Args&&... args );

/**
 * This factory function is used to call non overloaded member functions.
 * @param object Shared pointer to an object whose member function will be called.
 * @param f Pointer to a const member function.
 * @param args... Arguments of member function.
 * @return Unique pointer to task object.
 */
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask > create_task_ptr( const std::shared_ptr< Object >& object,
                                          ConstMemberFnPtr< Object, R, Params... > f,
                                          Args&&... args );

}  // namespace uni
