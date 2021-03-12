#include "uni/common/Runtime.hpp"

namespace uni
{
namespace common
{
TaskDispatcherBasic&
Runtime::task_dispatcher( )
{
    return m_dispatcher;
}

}  // namespace common
}  // namespace uni
