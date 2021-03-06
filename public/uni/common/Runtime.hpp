#pragma once

#include "uni/common/TaskDispatcher.hpp"

namespace uni
{
namespace common
{
// TODO: Separate interface from implementation.
class Runtime
{
public:
    TaskDispatcher& task_dispatcher( );

private:
    TaskDispatcher m_dispatcher{};
};
}  // namespace common
}  // namespace uni
