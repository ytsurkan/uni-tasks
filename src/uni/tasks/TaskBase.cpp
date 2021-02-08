#include "uni/tasks/TaskBase.hpp"

namespace uni
{
void
TaskBase::run_impl( )
{
    try
    {
        call_impl( );
        if ( m_promise )
        {
            m_promise->set_value( );
        }
    }
    catch ( const std::exception& )
    {
        // TODO: log exception
        if ( m_promise )
        {
            m_promise->set_exception( std::current_exception( ) );
        }
        throw;
    }
}

RequestId
TaskBase::request_id_impl( ) const
{
    return m_request_id;
}

void
TaskBase::set_request_id_impl( RequestId request_id )
{
    m_request_id = request_id;
}

SequenceId
TaskBase::sequence_id_impl( ) const
{
    return m_sequence_id;
}

void
TaskBase::set_due_time_impl( TimeInterval delay )
{
    m_due_time = std::chrono::steady_clock::now( ) + std::chrono::milliseconds( delay );
}

TimeInterval
TaskBase::due_time_impl( ) const
{
    using namespace std::chrono;
    return duration_cast< milliseconds >( m_due_time.time_since_epoch( ) ).count( );
}

TimeInterval
TaskBase::delay_impl( ) const
{
    const auto now = std::chrono::steady_clock::now( );
    const auto diff_ms
        = std::chrono::duration_cast< std::chrono::milliseconds >( m_due_time - now );
    if ( diff_ms.count( ) < 0 )
    {
        return 0;
    }
    return diff_ms.count( );
}

std::future< void >
TaskBase::get_future_impl( )
{
    if ( !m_promise )
    {
        m_promise = std::promise< void >( );
    }
    return m_promise->get_future( );
}
}  // namespace uni
