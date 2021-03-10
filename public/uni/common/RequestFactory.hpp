#pragma once

#include "uni/common/Request.hpp"
#include "uni/common/RequestCallbacks.hpp"

namespace uni
{
namespace common
{
Request create_request( RequestId id, RequestStartCallback start, RequestCancelCallback cancel );
}  // namespace common
}  // namespace uni
