#include "uni/common/RequestImpl.hpp"

namespace uni
{
namespace common
{
RequestImpl::RequestImpl( RequestId id,
                          RequestStartCallback start_callback,
                          RequestCancelCallback cancel_callback )
    : m_request_id{id}
    , m_start_callback{std::move( start_callback )}
    , m_cancel_callback{std::move( cancel_callback )}
{
}

RequestId
RequestImpl::request_id_impl( ) const
{
    return m_request_id;
}

void
RequestImpl::start_impl( )
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
RequestImpl::cancel_impl( )
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
