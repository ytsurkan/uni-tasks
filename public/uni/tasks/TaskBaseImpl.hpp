#pragma once

#include <chrono>
#include <future>

#include "uni/utils/Utils.hpp"

namespace uni
{
class TaskBaseImpl
{
public:
    TaskBaseImpl( ) = default;
    virtual ~TaskBaseImpl( ) = default;

    TaskBaseImpl( const TaskBaseImpl& ) = delete;
    TaskBaseImpl& operator=( const TaskBaseImpl& ) = delete;
    TaskBaseImpl( TaskBaseImpl&& ) noexcept = default;
    TaskBaseImpl& operator=( TaskBaseImpl&& ) noexcept = default;

public:
    void run_impl( );
    RequestId request_id_impl( ) const;
    void set_request_id_impl( RequestId request_id );
    SequenceId sequence_id_impl( ) const;
    void set_due_time_impl( TimeInterval delay );
    TimeInterval due_time_impl( ) const;
    TimeInterval delay_impl( ) const;
    std::future< void > get_future_impl( );

protected:
    virtual void call_impl( ) = 0;

private:
    std::chrono::time_point< std::chrono::steady_clock > m_due_time{
        std::chrono::steady_clock::now( )};
    RequestId m_request_id{0};
    SequenceId m_sequence_id{utils::create_sequence_id( )};
    std::promise< void > m_promise{};
};

}  // namespace uni
