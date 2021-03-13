#pragma once

#include "uni/tasks/details/TaskFactoryImpl.hpp"

namespace uni
{
template < typename Object, typename R, typename... Params, typename... Args >
std::unique_ptr< ITask >
create_task_ptr( Object* object, MemberFnPtr< Object, R, Params... > f, Args&&... args )
{
    return details::create_task_ptr_impl( object, f, std::forward< Args >( args )... );
}
}  // namespace uni
