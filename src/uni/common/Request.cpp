#include "uni/common/Request.hpp"
#include "uni/common/RequestImpl.hpp"

namespace uni
{
namespace common
{
Request::Request( RequestId id, RequestStartCallback start, RequestCancelCallback cancel )
    : m_impl{std::make_shared< RequestImpl >( id, start, cancel )}
{
}

RequestId
Request::get_request_id( ) const
{
    return m_impl->request_id_impl( );
}

void
Request::start( )
{
    m_impl->start_impl( );
}

void
Request::cancel( )
{
    m_impl->cancel_impl( );
}

}  // namespace common
}  // namespace uni
