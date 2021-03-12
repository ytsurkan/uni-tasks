#pragma once

#include <tuple>
#include <vector>

namespace integration
{
namespace uni
{
namespace common
{
inline std::vector< std::tuple< std::string, size_t > >
get_default_thread_pool_configuration( )
{
    return std::vector< std::tuple< std::string, size_t > >{{"background", 7}, {"client", 1}};
}
}  // namespace common
}  // namespace uni
}  // namespace integration
