#pragma once

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <set>

#include <iostream>

namespace uni
{
struct QueueClosedException : public std::exception
{
    const char*
    what( ) const noexcept override
    {
        return "QueueIsClosed";
    }
};

enum class OperationStatus
{
    success,
    closed
};


template < typename T = std::function< void( ) >, typename Container = std::deque< T > >
class Queue
{
public:
    Queue( ) = default;

    Queue( const Queue& ) = delete;
    Queue& operator=( const Queue& ) = delete;

    Queue( Queue&& ) = delete;
    Queue& operator=( Queue&& ) = delete;

    ~Queue( ) = default;

    void
    push( T& element )
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        throw_if_closed( );
        m_elements.push_back( element );
        m_cond.notify_one( );
    }

    void
    push( T&& element )
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        throw_if_closed( );
        m_elements.push_back( std::move( element ) );
        m_cond.notify_one( );
    }

    OperationStatus
    wait_pop( T& element )
    {
        std::unique_lock< std::mutex > lock( m_mutex );
        m_cond.wait( lock, [this]( ) { return is_not_empty_or_closed( ); } );
        if ( empty( lock ) )
        {
            return OperationStatus::closed;
        }
        element = std::move( m_elements.front( ) );
        m_elements.pop_front( );
        return OperationStatus::success;
    }

    ///@brief After calling close, queue doesn't wait for new elements and fails on submission of
    /// new elements.
    void
    close( )
    {
        {
            std::lock_guard< std::mutex > guard( m_mutex );
            m_closed = true;
        }
        m_cond.notify_all( );
    }

    bool
    closed( ) const
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        return m_closed;
    }

    bool
    empty( ) const
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        return m_elements.empty( );
    }

    template < typename Predicate >
    bool
    remove( Predicate&& predicate )
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        const auto pos = m_elements.erase( std::remove_if( std::begin( m_elements ),
                                                           std::end( m_elements ),
                                                           std::forward< Predicate >( predicate ) ),
                                           std::end( m_elements ) );
        return ( pos != std::end( m_elements ) );
    }

private:
    bool
    empty( std::unique_lock< std::mutex >& ) const noexcept
    {
        return m_elements.empty( );
    }

    bool
    is_not_empty_or_closed( ) const noexcept
    {
        return !m_elements.empty( ) || m_closed;
    }

    void
    throw_if_closed( ) const
    {
        if ( m_closed )
        {
            throw QueueClosedException( );
        }
    }

private:
    Container m_elements{};
    mutable std::mutex m_mutex{};
    std::condition_variable m_cond{};
    bool m_closed{false};
};

template < typename T, typename Cmp >
class Queue< T, std::set< T, Cmp > >
{
    using Container = std::set< T, Cmp >;

public:
    Queue( ) = default;

    Queue( const Queue& ) = delete;
    Queue& operator=( const Queue& ) = delete;

    Queue( Queue&& ) = delete;
    Queue& operator=( Queue&& ) = delete;

    ~Queue( ) = default;

    void
    push( T& element )
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        throw_if_closed( );
        m_elements.insert( element );
        m_cond.notify_one( );
    }

    void
    push( T&& element )
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        throw_if_closed( );
        m_elements.insert( std::move( element ) );
        m_cond.notify_one( );
    }

    template < typename Callback >
    OperationStatus
    wait_pop( T& element, Callback callback )
    {
        std::unique_lock< std::mutex > lock( m_mutex );
        while ( !closed( lock ) )
        {
            if ( !empty( lock ) )
            {
                const auto delay = get_delay_of_first_element( callback, lock );
                if ( delay <= 0 )
                {
                    element = std::move( m_elements.extract( m_elements.begin( ) ).value( ) );
                    return OperationStatus::success;
                }
                m_cond.wait_for( lock, std::chrono::milliseconds( delay ) );
            }
            else
            {
                m_cond.wait( lock, [&]( ) { return is_not_empty_or_closed( ); } );
            }
        }

        // Queue is closed. Extract remaining elements and do not accept insertions of new elements.
        while ( !empty( lock ) )
        {
            const auto delay = get_delay_of_first_element( callback, lock );
            if ( delay <= 0 )
            {
                element = std::move( m_elements.extract( m_elements.begin( ) ).value( ) );
                return OperationStatus::success;
            }
            m_cond.wait_for( lock, std::chrono::milliseconds( delay ) );
        }
        return OperationStatus::closed;
    }

    // TODO: Merge both wait_pop functions to one.
    template < typename Callback >
    std::optional< T >
    wait_pop( Callback callback )
    {
        std::unique_lock< std::mutex > lock( m_mutex );
        while ( !closed( lock ) )
        {
            if ( !empty( lock ) )
            {
                const auto delay = get_delay_of_first_element( callback, lock );
                if ( delay <= 0 )
                {
                    return std::move( m_elements.extract( m_elements.begin( ) ).value( ) );
                }
                m_cond.wait_for( lock, std::chrono::milliseconds( delay ) );
            }
            else
            {
                m_cond.wait( lock, [&]( ) { return is_not_empty_or_closed( ); } );
            }
        }

        // Queue is closed. Extract remaining elements and do not accept insertions of new elements.
        while ( !empty( lock ) )
        {
            const auto delay = get_delay_of_first_element( callback, lock );
            if ( delay <= 0 )
            {
                return std::move( m_elements.extract( m_elements.begin( ) ).value( ) );
            }
            m_cond.wait_for( lock, std::chrono::milliseconds( delay ) );
        }
        return {};
    }
    ///@brief After calling close, queue doesn't wait for new elements and fails on insertions of
    /// new elements.
    void
    close( )
    {
        {
            std::lock_guard< std::mutex > guard( m_mutex );
            m_closed = true;
        }
        m_cond.notify_all( );
    }

    bool
    closed( ) const
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        return closed( guard );
    }

    bool
    empty( ) const
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        return m_elements.empty( );
    }

    template < typename Predicate >
    bool
    remove( Predicate&& predicate )
    {
        bool removed = false;
        std::lock_guard< std::mutex > guard( m_mutex );
        for ( auto it = m_elements.begin( ); it != m_elements.end( ); )
        {
            if ( predicate( *it ) )
            {
                it = m_elements.erase( it );
                removed = true;
            }
            else
            {
                ++it;
            }
        }
        return removed;
    }

private:
    bool
    empty( std::unique_lock< std::mutex >& ) const noexcept
    {
        return m_elements.empty( );
    }

    bool
    closed( std::lock_guard< std::mutex >& ) const
    {
        return m_closed;
    }

    bool
    closed( std::unique_lock< std::mutex >& ) const
    {
        return m_closed;
    }

    bool
    is_not_empty_or_closed( ) const noexcept
    {
        return !m_elements.empty( ) || m_closed;
    }

    void
    throw_if_closed( ) const
    {
        if ( m_closed )
        {
            throw QueueClosedException( );
        }
    }

    template < typename Callback >
    auto
    get_delay_of_first_element( const Callback& callback, const std::unique_lock< std::mutex >& )
        -> decltype( callback( std::declval< T >( ) ) ) const
    {
        const T& element = *( m_elements.begin( ) );
        return callback( element );
    };

private:
    Container m_elements{};
    mutable std::mutex m_mutex{};
    std::condition_variable m_cond{};
    bool m_closed{false};
};

}  // namespace uni
