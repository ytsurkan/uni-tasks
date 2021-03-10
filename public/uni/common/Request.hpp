#pragma once

#include <memory>

#include "uni/Types.hpp"
#include "uni/common/RequestCallbacks.hpp"

namespace uni
{
namespace common
{
class RequestImpl;
}  // namespace common
}  // namespace uni

namespace uni
{
namespace common
{
class Request
{
public:
    Request( RequestId id, RequestStartCallback start, RequestCancelCallback cancel );

    RequestId get_request_id( ) const;
    void start( );
    void cancel( );

private:
    std::shared_ptr< RequestImpl > m_impl;
};

}  // namespace common
}  // namespace uni
