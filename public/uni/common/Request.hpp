#pragma once

#include <functional>
#include <future>

#include "uni/Types.hpp"

namespace uni
{
namespace common
{
class Request
{
public:
    using StartCallback = std::function< std::shared_future< void >( ) >;
    using CancelCallback = std::function< void( std::shared_future< void > ) >;

    Request( RequestId id, StartCallback start_callback, CancelCallback cancel_callback );

    RequestId request_id( ) const;
    void start( );
    void cancel( );

private:
    RequestId m_request_id{0};
    StartCallback m_start_callback;
    CancelCallback m_cancel_callback;
    std::atomic< bool > m_started{false};
    std::shared_future< void > m_future;
};

}  // namespace common
}  // namespace uni
