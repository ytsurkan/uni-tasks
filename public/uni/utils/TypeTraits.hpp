#pragma once

#include <memory>
#include <type_traits>

namespace uni
{
namespace utils
{
namespace details
{
template < typename T >
struct IsUniquePointerImpl : std::false_type
{
};

template < typename T >
struct IsUniquePointerImpl< std::unique_ptr< T > > : std::true_type
{
    using Type = T;
};
}  // namespace details

template < typename T >
using IsUniquePointer
    = details::IsUniquePointerImpl< std::remove_cv_t< std::remove_reference_t< T > > >;
}  // namespace utils
}  // namespace uni
