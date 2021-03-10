#include "uni/common/RequestFactory.hpp"

namespace uni
{
namespace common
{
Request
create_request( RequestId id, RequestStartCallback start, RequestCancelCallback cancel )
{
    return Request{id, start, cancel};
}
}  // namespace common

}  // namespace uni
