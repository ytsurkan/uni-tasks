#pragma once

#include <atomic>
#include <functional>
#include <memory>

#include "uni/Types.hpp"
#include "uni/common/Runtime.hpp"

namespace uni
{
namespace common
{
class Runtime;

// TODO: Separate interface from implementation.
class PollEngine
{
public:
    using Callback = std::function< void( ) >;

    PollEngine( const Runtime& runtime, const std::string& thread_pool_name );
    void start( );
    void stop( );
    void set_poll_interval( TimeInterval poll_interval_ms );

    template < typename F >
    void set_poll_callback( F&& callback );

private:
    void poll( );
    void run( );

private:
    Runtime m_runtime;
    const std::string m_thread_pool_name{};
    std::atomic< TimeInterval > m_poll_interval_ms{0u};
    std::atomic< bool > m_running{false};
    Callback m_poll_callback{};
    std::atomic< RequestId > m_poll_request_id{0u};
};

template < typename F >
void
PollEngine::set_poll_callback( F&& callback )
{
    m_poll_callback = std::forward< F >( callback );
}

}  // namespace common
}  // namespace uni
