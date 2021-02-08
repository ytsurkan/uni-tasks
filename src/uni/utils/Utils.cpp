#include <atomic>

#include "uni/utils/Utils.hpp"

namespace uni
{
RequestId
utils::create_request_id( )
{
    static std::atomic< RequestId > s_REQUEST_ID{0};
    return ++s_REQUEST_ID;
}

SequenceId
utils::create_sequence_id( )
{
    static std::atomic< SequenceId > s_SEQUENCE_ID{0};
    return ++s_SEQUENCE_ID;
}

}  // namespace uni
