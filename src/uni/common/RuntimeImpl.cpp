#include "uni/common/RuntimeImpl.hpp"

namespace uni
{
namespace common
{
RuntimeImpl::RuntimeImpl(
    const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations )
    : m_dispatcher{thread_pool_configurations}
{
}

TaskDispatcherBasic&
RuntimeImpl::task_dispatcher_basic_impl( )
{
    return m_dispatcher;
}

}  // namespace common
}  // namespace uni
