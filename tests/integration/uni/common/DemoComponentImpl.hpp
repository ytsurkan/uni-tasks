#pragma once

#include <memory>

#include "uni/common/Request.hpp"
#include "uni/common/Runtime.hpp"

namespace integration
{
namespace uni
{
namespace common
{
class DemoComponentListener;

class DemoComponentImpl : public std::enable_shared_from_this< DemoComponentImpl >
{
public:
    DemoComponentImpl( const ::uni::common::Runtime& runtime, DemoComponentListener& listener );

    ::uni::common::Request start_calculation_1( int32_t p1, const std::string& p2 );

private:
    std::shared_future< void > schedule_calculation_1( ::uni::RequestId id,
                                                       int32_t p1,
                                                       const std::string& p2 );
    void do_calculation_1( ::uni::RequestId id, int32_t p1, const std::string& p2 );
    void poll_calculation_1( );
    void cancel_calculation_1( );
    void reset_calculation_1( );
    void do_calculation_1_internal( ::uni::RequestId id, int32_t p1 );
    void schedule_cancel_calculation_1( ::uni::RequestId id,
                                        std::shared_future< void > task_future );
    std::weak_ptr< DemoComponentImpl > weak_ptr( );

private:
    ::uni::common::Runtime m_runtime;
    DemoComponentListener& m_listener;
    static constexpr size_t m_max_iterations_per_poll{10};
    std::atomic< bool > m_calculate_1_cancelled{false};
};

}  // namespace common
}  // namespace uni
}  // namespace integration
