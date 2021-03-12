#pragma once

#include <cstddef>
#include <cstdint>

namespace uni
{
// TODO: add tagged RequestId, SequenceId, create Runtime class and move create_request_id() there
using RequestId = size_t;
using SequenceId = size_t;

using TimeInterval = int64_t;

template < typename Object, typename R, typename... Args >
using MemberFnPtr = R ( Object::* )( Args... );

template < typename Object, typename R, typename... Args >
using ConstMemberFnPtr = R ( Object::* )( Args... ) const;

/**
 * The rationale for why the MemberFnPtrWrapper was created.
 * void f1( int ) {}
 * void f1( int, int ) { }
 * std::function< decltype( f1 ) > func;
 * Compilation fails with error error: decltype cannot resolve address of overloaded function,
 * because it's not clear which function should be taken, for overloaded function the
 * function prototype should be explicitly specified, something like this
 * std::function< void( int ) > func;
 */
template < typename F >
struct MemberFnPtrWrapper;

template < typename Object, typename R, typename... Args >
struct MemberFnPtrWrapper< R ( Object::* )( Args... ) >
{
    using Type = R ( Object::* )( Args... );

    MemberFnPtrWrapper( Type ptr )
        : m_ptr{ptr}
    {
    }

    Type operator*( ) const noexcept
    {
        return m_ptr;
    }

private:
    Type m_ptr{nullptr};
};

template < typename Object, typename R, typename... Args >
struct MemberFnPtrWrapper< R ( Object::* )( Args... ) const >
{
    using Type = R ( Object::* )( Args... ) const;

    MemberFnPtrWrapper( Type ptr )
        : m_ptr{ptr}
    {
    }

    Type operator*( ) const noexcept
    {
        return m_ptr;
    }

private:
    Type m_ptr{nullptr};
};
}  // namespace uni
