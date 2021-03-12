#pragma once

#include "uni/common/TaskDispatcherBasic.hpp"

namespace uni
{
namespace common
{
class RuntimeImpl
{
public:
    explicit RuntimeImpl(
        const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations );

    TaskDispatcherBasic& task_dispatcher_basic_impl( );

private:
    TaskDispatcherBasic m_dispatcher;
};

}  // namespace common
}  // namespace uni
