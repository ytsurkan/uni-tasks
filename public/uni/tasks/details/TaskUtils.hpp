#pragma once
#include "uni/Types.hpp"
#include "uni/tasks/ITask.hpp"
#include "uni/utils/TypeTraits.hpp"

namespace uni
{
namespace tasks
{
template < typename T, std::enable_if_t< !::uni::utils::IsUniquePointer< T >::value, bool > = true >
TimeInterval
get_task_delay( const T& task )
{
    return static_cast< const uni::ITask& >( task ).delay( );
};

template < typename T, std::enable_if_t< ::uni::utils::IsUniquePointer< T >::value, bool > = true >
TimeInterval
get_task_delay( const T& task )
{
    return task->delay( );
};

}  // namespace tasks
}  // namespace uni
