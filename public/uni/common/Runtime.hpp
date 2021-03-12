#pragma once

#include "uni/common/TaskDispatcherBasic.hpp"

namespace uni
{
namespace common
{
// TODO: Separate interface from implementation.
class Runtime
{
public:
    TaskDispatcherBasic& task_dispatcher( );

private:
    TaskDispatcherBasic m_dispatcher{};
};
}  // namespace common
}  // namespace uni
