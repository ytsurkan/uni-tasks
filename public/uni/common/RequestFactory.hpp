#pragma once

#include "uni/common/Request.hpp"

namespace uni
{
namespace common
{
inline Request
create_request( RequestId id, Request::StartCallback start, Request::CancelCallback cancel )
{
    return {id, start, cancel};
}

}  // namespace common
}  // namespace uni
