#pragma once

#include "uni/common/Runtime.hpp"

namespace uni
{
namespace common
{
Runtime create_runtime(
    const std::vector< std::tuple< std::string, size_t > >& thread_pool_configurations );
}  // namespace common
}  // namespace uni
