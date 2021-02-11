#pragma once

#include "uni/tasks/ITask.hpp"

namespace uni
{
struct TaskPtrCmp
{
    bool
    operator( )( const std::unique_ptr< ITask >& lhs, const std::unique_ptr< ITask >& rhs ) const
    {
        return lhs->due_time( ) != rhs->due_time( ) ? lhs->due_time( ) < rhs->due_time( )
                                                    : lhs->sequence_id( ) < rhs->sequence_id( );
    }
};

struct TaskCmp
{
    bool
    operator( )( const ITask& lhs, const ITask& rhs ) const
    {
        return lhs.due_time( ) != rhs.due_time( ) ? lhs.due_time( ) < rhs.due_time( )
                                                  : lhs.sequence_id( ) < rhs.sequence_id( );
    }
};
}  // namespace uni
