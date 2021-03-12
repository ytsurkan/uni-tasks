#pragma once

#include "uni/Types.hpp"
#include "uni/common/Runtime.hpp"

namespace uni
{
namespace common
{
class PollEngineImpl
{
public:
    using Callback = std::function< void( ) >;

    PollEngineImpl( const Runtime& runtime, const std::string& thread_pool_name );
    void start_impl( );
    void stop_impl( );
    void set_poll_interval_impl( TimeInterval poll_interval_ms );
    void set_poll_callback_impl( Callback callback );

private:
    void poll_impl( );
    void run_impl( );

private:
    Runtime m_runtime;
    const std::string m_thread_pool_name{};
    std::atomic< TimeInterval > m_poll_interval_ms{0u};
    std::atomic< bool > m_running{false};
    Callback m_poll_callback{};
    std::atomic< RequestId > m_poll_request_id{0u};
};

}  // namespace common
}  // namespace uni
