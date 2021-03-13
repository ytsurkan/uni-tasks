#pragma once

#include "uni/tasks/details/TaskFactoryBasic.inl"

namespace uni
{
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
}  // namespace uni
