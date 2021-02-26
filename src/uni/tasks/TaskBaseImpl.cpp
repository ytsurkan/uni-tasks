#include "uni/tasks/TaskBaseImpl.hpp"

namespace uni
{
void
TaskBaseImpl::run_impl( )
{
    try
    {
        call_impl( );
        m_promise.set_value( );
    }
    catch ( const std::exception& )
    {
        // TODO: log exception
        m_promise.set_exception( std::current_exception( ) );
        throw;
    }
}

RequestId
TaskBaseImpl::request_id_impl( ) const
{
    return m_request_id;
}

void
TaskBaseImpl::set_request_id_impl( RequestId request_id )
{
    m_request_id = request_id;
}

SequenceId
TaskBaseImpl::sequence_id_impl( ) const
{
    return m_sequence_id;
}

void
TaskBaseImpl::set_due_time_impl( TimeInterval delay )
{
    using namespace std::chrono;
    m_due_time = steady_clock::now( ) + milliseconds( delay );
}

TimeInterval
TaskBaseImpl::due_time_impl( ) const
{
    using namespace std::chrono;
    return duration_cast< milliseconds >( m_due_time.time_since_epoch( ) ).count( );
}

TimeInterval
TaskBaseImpl::delay_impl( ) const
{
    using namespace std::chrono;

    const auto now = steady_clock::now( );
    const auto diff_ms = duration_cast< milliseconds >( m_due_time - now );
    if ( diff_ms.count( ) < 0 )
    {
        return 0;
    }
    return diff_ms.count( );
}

std::future< void >
TaskBaseImpl::get_future_impl( )
{
    return m_promise.get_future( );
}

}  // namespace uni
