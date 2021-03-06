#include "uni/common/Runtime.hpp"
#include "uni/common/RuntimeImpl.hpp"

namespace uni
{
namespace common
{
Runtime::Runtime(
    const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations )
    : m_impl{std::make_shared< RuntimeImpl >( thread_pool_configurations )}
{
}

TaskDispatcherBasic&
Runtime::task_dispatcher_basic( )
{
    return m_impl->task_dispatcher_basic_impl( );
}

}  // namespace common
}  // namespace uni
