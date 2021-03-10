#pragma once

#include <functional>
#include <future>

namespace uni
{
namespace common
{
using RequestStartCallback = std::function< std::shared_future< void >( ) >;
using RequestCancelCallback = std::function< void( std::shared_future< void > ) >;
}  // namespace common
}  // namespace uni
