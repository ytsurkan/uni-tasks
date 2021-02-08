#pragma once

#include <chrono>
#include <future>
#include <optional>

#include "uni/tasks/ITask.hpp"
#include "uni/utils/Utils.hpp"

namespace uni
{
class TaskBase : public ITask
{
public:
    using PromiseType = std::promise< void >;

public:
    TaskBase( ) = default;
    virtual ~TaskBase( ) = default;

    TaskBase( const TaskBase& ) = delete;
    TaskBase& operator=( const TaskBase& ) = delete;
    TaskBase( TaskBase&& ) noexcept = default;
    TaskBase& operator=( TaskBase&& ) noexcept = default;

protected:
    virtual void call_impl( ) = 0;

    /// ITask interface implementation
    void run_impl( ) override;
    RequestId request_id_impl( ) const override;
    void set_request_id_impl( RequestId request_id ) override;
    SequenceId sequence_id_impl( ) const override;
    void set_due_time_impl( TimeInterval delay ) override;
    TimeInterval due_time_impl( ) const override;
    TimeInterval delay_impl( ) const override;
    std::future< void > get_future_impl( ) override;

protected:
    std::chrono::time_point< std::chrono::steady_clock > m_due_time{
        std::chrono::steady_clock::now( )};
    RequestId m_request_id{0};
    SequenceId m_sequence_id{utils::create_sequence_id( )};
    // TODO: check if optional is really needed
    std::optional< PromiseType > m_promise;
};

}  // namespace uni
