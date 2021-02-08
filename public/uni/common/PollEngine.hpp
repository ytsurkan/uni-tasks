#pragma once

#include <atomic>
#include <functional>

#include "uni/Types.hpp"

namespace uni
{
namespace common
{
class Runtime;

class PollEngine
{
public:
    using Callback = std::function< void( ) >;

    PollEngine( Runtime* runtime, const std::string& thread_pool_name = "background" );
    void start( );
    void stop( );
    void set_poll_interval( TimeInterval poll_interval_ms );

    template < typename F >
    void
    set_poll_callback( F&& callback )
    {
        m_poll_callback = std::forward< F >( callback );
    }

protected:
    virtual void
    poll_impl( )
    {
    }

private:
    void poll( );
    void run( );

private:
    Runtime* const m_runtime{nullptr};
    const std::string m_thread_pool_name;
    std::atomic< TimeInterval > m_poll_interval_ms{0u};
    std::atomic< bool > m_running{false};
    Callback m_poll_callback;
    RequestId m_poll_request_id{0u};
};

}  // namespace common
}  // namespace uni
