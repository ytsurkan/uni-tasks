#pragma once

#include <memory>

#include "uni/common/RequestFactory.hpp"

class DemoComponentListener;
namespace uni
{
namespace common
{
class Runtime;
}
}

class DemoComponentImpl : public std::enable_shared_from_this< DemoComponentImpl >
{
public:
    DemoComponentImpl( uni::common::Runtime& runtime, DemoComponentListener& listener );

    uni::common::Request calculate_1_impl( int p1, const std::string& p2 );

private:
    std::shared_future< void > schedule_calculate_1( uni::RequestId id,
                                                     int p1,
                                                     const std::string& p2 );
    void do_calculate_1( uni::RequestId id, int p1, const std::string& p2 );
    void poll_calculate_1( );
    void cancel_calculate_1( );
    void reset_calculate_1( );
    void do_calculate_1_internal( uni::RequestId id, int p1 );
    void schedule_cancel_calculate_1( uni::RequestId id, std::shared_future< void > task_future );
    std::weak_ptr< DemoComponentImpl > weak_ptr( );

private:
    uni::common::Runtime& m_runtime;
    DemoComponentListener& m_listener;
    static constexpr size_t m_max_iterations_per_poll{10};
    std::atomic< bool > m_calculate_1_cancelled{false};
};
