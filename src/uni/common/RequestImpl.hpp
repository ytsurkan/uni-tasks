#pragma once

#include "uni/Types.hpp"
#include "uni/common/RequestCallbacks.hpp"

namespace uni
{
namespace common
{
class RequestImpl
{
public:
    RequestImpl( RequestId id,
                 RequestStartCallback start_callback,
                 RequestCancelCallback cancel_callback );

    RequestId request_id_impl( ) const;
    void start_impl( );
    void cancel_impl( );

private:
    RequestId m_request_id{0};
    RequestStartCallback m_start_callback;
    RequestCancelCallback m_cancel_callback;
    std::atomic< bool > m_started{false};
    std::shared_future< void > m_future{};
};

}  // namespace common
}  // namespace uni
