#pragma once

#include "uni/common/TaskDispatcherBasic.hpp"

namespace uni
{
namespace common
{
class RuntimeImpl;
}  // namespace common
}  // namespace uni

namespace uni
{
namespace common
{
class Runtime
{
public:
    explicit Runtime(
        const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations );

    TaskDispatcherBasic& task_dispatcher_basic( );

private:
    std::shared_ptr< RuntimeImpl > m_impl;
};

}  // namespace common
}  // namespace uni
