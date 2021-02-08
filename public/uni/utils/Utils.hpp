#pragma once

#include <stdexcept>
#include <utility>

#include "uni/Types.hpp"

namespace uni
{
namespace utils
{
RequestId create_request_id( );
SequenceId create_sequence_id( );

// namespace details
//{
// template<typename T>
// struct IsSharedPointerImpl : std::false_type
//{};

// template<typename T>
// struct IsSharedPointerImpl<std::shared_ptr<T>> : std::true_type
//{
//    using Type = T;
//};
//}  // namespace details
// template<typename T>
// using IsSharedPointer = details::IsSharedPointerImpl<T>;

// TODO: Use C++17 std::apply, std::invoke instead of apply, invoke declared above.

namespace details
{
template < typename Base, typename R, typename Object, typename... Args >
decltype( auto )
invoke_impl( R Base::*f, Object&& object, Args&&... args )
{
    static_assert( std::is_member_function_pointer< decltype( f ) >::value,
                   "Expect member function pointer" );
    static_assert( std::is_base_of< Base, std::decay_t< Object > >::value, "Expect base class" );

    return ( object.*f )( std::forward< Args >( args )... );
}

template < typename F, typename... Args >
decltype( auto )
invoke_impl( F&& f, Args&&... args )
{
    return f( std::forward< Args >( args )... );
}
}  // namespace details

template < typename F, typename... Args >
decltype( auto )
invoke( F&& f, Args&&... args )
{
    return details::invoke_impl( std::forward< F >( f ), std::forward< Args >( args )... );
}

namespace details
{
template < typename F, typename Tuple, size_t... Indices >
decltype( auto )
apply_impl( F&& f, Tuple&& tuple, std::index_sequence< Indices... > )
{
    return utils::invoke( std::forward< F >( f ), std::get< Indices >( tuple )... );
}
}  // namespace details

template < typename F, typename Tuple >
decltype( auto )
apply( F&& f, Tuple&& tuple )
{
    return details::apply_impl(
        std::forward< F >( f ),
        std::forward< Tuple >( tuple ),
        std::make_index_sequence< std::tuple_size< std::remove_reference_t< Tuple > >::value >{} );
}

template < typename Object, typename R, typename... Params >
MemFnPtrWrapper< R ( Object::* )( Params... ) >
ptr2wrapper( MemFnPtr< Object, R, Params... > ptr )
{
    return ptr;
}

template < typename Object, typename R, typename... Params >
MemFnPtrWrapper< R ( Object::* )( Params... ) const >
ptr2wrapper( ConstMemFnPtr< Object, R, Params... > ptr )
{
    return ptr;
}

inline void
Expects( bool condition, const char* msg )
{
    if ( !condition )
    {
        throw std::logic_error( msg );
    }
}
}  // namespace utils

}  // namespace uni
