#include "uni/common/Request.hpp"

namespace uni
{
namespace common
{
Request::Request( RequestId id,
                  Request::StartCallback start_callback,
                  Request::CancelCallback cancel_callback )
    : m_request_id{id}
    , m_start_callback{std::move( start_callback )}
    , m_cancel_callback{std::move( cancel_callback )}
{
}

RequestId
Request::request_id( ) const
{
    return m_request_id;
}

void
Request::start( )
{
    if ( m_started )
    {
        throw std::logic_error( "Request already started!" );
    }
    m_started = true;
    m_future = m_start_callback( );
    m_start_callback = {};
}

void
Request::cancel( )
{
    if ( !m_started )
    {
        throw std::logic_error( "Request already cancelled!" );
    }
    m_started = false;
    m_cancel_callback( std::move( m_future ) );
    m_cancel_callback = {};
}
}  // namespace common
}  // namespace uni
