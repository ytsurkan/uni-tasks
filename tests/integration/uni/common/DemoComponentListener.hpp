#pragma once

#include <string>

#include "uni/Types.hpp"

// TODO: move to somewhere
#define UNUSED( x ) (void)( x );

namespace integration
{
namespace uni
{
namespace common
{
class DemoComponentListener
{
public:
    virtual void
    on_calculate_started( ::uni::RequestId id )
    {
        UNUSED( id );
    }

    virtual void
    on_calculate_progress( ::uni::RequestId id, size_t progress )
    {
        UNUSED( id );
        UNUSED( progress );
    }

    virtual void
    on_calculation_done( ::uni::RequestId id, int result1, const std::string& result2, bool status )
    {
        UNUSED( id );
        UNUSED( result1 );
        UNUSED( result2 );
        UNUSED( status );
    }

    virtual ~DemoComponentListener() = default;
};

}  // namespace common
}  // namespace uni
}  // namespace integration
