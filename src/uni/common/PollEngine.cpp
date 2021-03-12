#include "uni/common/PollEngine.hpp"
#include "uni/common/PollEngineImpl.hpp"

namespace uni
{
namespace common
{
PollEngine::PollEngine( const Runtime& runtime, const std::string& thread_pool_name )
    : m_impl{std::make_shared< PollEngineImpl >( runtime, thread_pool_name )}
{
}

void
PollEngine::start( )
{
    m_impl->start_impl( );
}

void
PollEngine::stop( )
{
    m_impl->stop_impl( );
}

void
PollEngine::set_poll_interval( TimeInterval poll_interval_ms )
{
    m_impl->set_poll_interval_impl( poll_interval_ms );
}

void
PollEngine::set_poll_callback( Callback callback )
{
    m_impl->set_poll_callback_impl( std::move( callback ) );
}

}  // namespace common
}  // namespace uni
