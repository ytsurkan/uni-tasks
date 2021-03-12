#pragma once

#include <functional>
#include <memory>

#include "uni/common/Runtime.hpp"

namespace uni
{
namespace common
{
class PollEngineImpl;
}  // namespace common
}  // namespace uni

namespace uni
{
namespace common
{
class PollEngine
{
public:
    using Callback = std::function< void( ) >;

    PollEngine( const Runtime& runtime, const std::string& thread_pool_name );
    void start( );
    void stop( );
    void set_poll_interval( TimeInterval poll_interval_ms );
    void set_poll_callback( Callback callback );

private:
    std::shared_ptr< PollEngineImpl > m_impl;
};

}  // namespace common
}  // namespace uni
