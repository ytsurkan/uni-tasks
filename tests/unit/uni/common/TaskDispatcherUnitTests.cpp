#include "uni/common/TaskDispatcher.hpp"

#define ASSERT( x )                                                                  \
    do                                                                               \
    {                                                                                \
        if ( !( x ) )                                                                \
        {                                                                            \
            std::fprintf( stderr, "Assertion failed: %s:%i\n", __FILE__, __LINE__ ); \
            std::fflush( stderr );                                                   \
        }                                                                            \
    } while ( 0 )

#define ASSERT_EQ( lhs, rhs ) ASSERT( lhs == rhs )

#define ASSERT_LE( lhs, rhs ) ASSERT( lhs <= rhs )

namespace
{
class Listener
{
public:
    static constexpr size_t s_magic_number = 13;

public:
    void
    on_calculation_done( int value )
    {
        m_result = value;
    }

    void
    on_result( int value1, int value2 )
    {
        m_result = value1 + value2;
    }

    void
    on_result( int value1, int value2 ) const
    {
        m_result = value1 + value2 + s_magic_number;
    }

    void
    on_result( int value1, int value2, std::promise< size_t > promise )
    {
        m_result = value1 + value2;
        m_promise = std::move( promise );
        m_promise.set_value( m_result );
    }

    std::promise< size_t > m_promise;
    mutable int m_result{};
};

template < typename F >
struct FunctionWrapper;

template < typename R, typename... Args >
struct FunctionWrapper< R( Args... ) >
{
    FunctionWrapper( const std::function< R( Args... ) >& f, R& result )
        : m_f{f}
        , m_result{result}
    {
    }

    R
    operator( )( Args... args )
    {
        m_result = m_f( std::forward< Args >( args )... );
        return m_result;
    }

    std::function< R( Args... ) > m_f;
    R& m_result;
};

int
add( int first, int second )
{
    return first + second;
}
}  // namespace

void
test_execute_lambda( )
{
    const auto f = []( int a, int b ) {
        const int r = a + b;
        ASSERT_EQ( r, ( 10 + 10 ) );
        return r;
    };

    uni::TaskDispatcher dispatcher;
    dispatcher.start( );

    bool async_executed{false};
    const auto call_wrapper = [&]( int x, int y ) {
        const bool executed = dispatcher.dispatch_or_execute( "background", f, x, y );
        ASSERT_EQ( executed, true );
        async_executed = true;
    };

    const int a = 10;
    const bool sync_executed = dispatcher.dispatch_or_execute( "background", call_wrapper, a, 10 );
    dispatcher.stop( );

    ASSERT_EQ( sync_executed, false );
    ASSERT_EQ( async_executed, true );
}

void
test_dispatch_lambda( )
{
    bool async_executed{false};
    const auto f = [&]( int a, int b ) {
        const int r = a + b;
        ASSERT_EQ( r, ( 10 + 10 ) );
        async_executed = true;
        return r;
    };

    uni::TaskDispatcher dispatcher;
    dispatcher.start( );

    const bool sync_executed = dispatcher.dispatch_or_execute( "background", f, 10, 10 );
    ASSERT_EQ( sync_executed, false );

    dispatcher.dispatch( "background", f, 10, 10 );
    dispatcher.stop( );

    ASSERT_EQ( sync_executed, false );
    ASSERT_EQ( async_executed, true );
}

void
test_execute_function( )
{
    int result = 0;
    FunctionWrapper< decltype( add ) > fw( add, result );

    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );

        bool async_executed{false};
        const auto call_wrapper = [&]( int x, int y ) {
            const bool executed = dispatcher.dispatch_or_execute( "background", fw, x, y );
            ASSERT_EQ( executed, true );
            async_executed = true;
        };

        const int a = 1;
        const int b = 2;
        dispatcher.dispatch_or_execute( "background", call_wrapper, a, b );
        dispatcher.stop( );

        ASSERT_EQ( async_executed, true );
    }

    const int expected = ( 1 + 2 );
    ASSERT_EQ( expected, result );
}

void
test_dispatch_function( )
{
    int result1 = 0;
    int result2 = 0;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );

        FunctionWrapper< decltype( add ) > fw( add, result1 );
        const bool executed = dispatcher.dispatch_or_execute( "background", fw, 1, 2 );
        ASSERT_EQ( executed, false );

        dispatcher.dispatch(
            "background", FunctionWrapper< decltype( add ) >( add, result2 ), 3, 4 );
        dispatcher.stop( );
    }

    const int expected1 = ( 1 + 2 );
    ASSERT_EQ( expected1, result1 );

    const int expected2 = ( 3 + 4 );
    ASSERT_EQ( expected2, result2 );
}

void
test_execute_memfn( )
{
    Listener foo;
    const int expected = 42;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );

        auto call_wrapper = [&]( int a ) {
            const bool executed = dispatcher.dispatch_or_execute(
                "background", &foo, &Listener::on_calculation_done, a );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", call_wrapper, expected );
        dispatcher.stop( );
    }

    ASSERT_EQ( expected, foo.m_result );
}

void
test_dispatch_memfn( )
{
    Listener foo1;
    const int expected1 = 42;
    Listener foo2;
    const int expected2 = 13;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );

        const bool executed = dispatcher.dispatch_or_execute(
            "background", &foo1, &Listener::on_calculation_done, expected1 );
        ASSERT_EQ( executed, false );

        dispatcher.dispatch( "background", &foo2, &Listener::on_calculation_done, expected2 );
        dispatcher.stop( );
    }

    ASSERT_EQ( expected1, foo1.m_result );
    ASSERT_EQ( expected2, foo2.m_result );
}

void
test_execute_const_memfn( )
{
    Listener foo;
    const int a = 42;
    const int b = 1;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );
        auto call_wrapper = [&]( int x, int y ) {
            const bool executed = dispatcher.dispatch_or_execute(
                "background",
                &foo,
                uni::MemFnPtrWrapper< void ( Listener::* )( int, int ) const >(
                    &Listener::on_result ),
                x,
                y );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", call_wrapper, a, b );
        dispatcher.stop( );
    }

    const int expected = ( a + b + Listener::s_magic_number );
    ASSERT_EQ( expected, foo.m_result );
}

void
test_dispatch_const_memfn( )
{
    Listener foo1;
    Listener foo2;
    const int a = 42;
    const int b = 1;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );

        const bool executed = dispatcher.dispatch_or_execute(
            "background",
            &foo1,
            uni::MemFnPtrWrapper< void ( Listener::* )( int, int ) const >( &Listener::on_result ),
            a,
            b );
        ASSERT_EQ( executed, false );

        dispatcher.dispatch(
            "background",
            &foo2,
            uni::MemFnPtrWrapper< void ( Listener::* )( int, int ) const >( &Listener::on_result ),
            a,
            b );
        dispatcher.stop( );
    }

    const int expected = ( a + b + Listener::s_magic_number );
    ASSERT_EQ( expected, foo1.m_result );
    ASSERT_EQ( expected, foo2.m_result );
}

void
test_execute_memfn_of_sharedptr_object( )
{
    const auto sp = std::make_shared< Listener >( );
    const int expected = 42;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );
        auto call_wrapper = [&]( int x ) {
            const bool executed = dispatcher.dispatch_or_execute(
                "background", sp, &Listener::on_calculation_done, x );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", std::move( call_wrapper ), expected );
        dispatcher.stop( );
    }

    ASSERT_EQ( expected, sp->m_result );
}

void
test_dispatch_memfn_of_sharedptr_object( )
{
    const auto sp = std::make_shared< Listener >( );
    const int expected = 42;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );
        const bool executed = dispatcher.dispatch_or_execute(
            "background", sp, &Listener::on_calculation_done, expected );
        dispatcher.stop( );
        ASSERT_EQ( executed, false );
    }

    ASSERT_EQ( expected, sp->m_result );
}

void
test_execute_const_memfn_of_sharedptr_object( )
{
    const auto sp = std::make_shared< Listener >( );
    const int a = 42;
    const int b = 1;
    {
        uni::TaskDispatcher dispatcher;
        dispatcher.start( );
        auto call_wrapper = [&]( int x, int y ) {
            const bool executed = dispatcher.dispatch_or_execute(
                "background",
                sp,
                uni::MemFnPtrWrapper< void ( Listener::* )( int, int ) const >(
                    &Listener::on_result ),
                x,
                y );
            ASSERT_EQ( executed, true );
        };

        dispatcher.dispatch_or_execute( "background", call_wrapper, a, b );
        dispatcher.stop( );
    }

    const int expected = ( a + b + Listener::s_magic_number );
    ASSERT_EQ( expected, sp->m_result );
}

void
task_dispatcher_tests( )
{
    test_execute_lambda( );
    test_dispatch_lambda( );
    test_execute_function( );
    test_dispatch_function( );
    test_execute_memfn( );
    test_dispatch_memfn( );
    test_execute_const_memfn( );
    test_dispatch_const_memfn( );
    test_execute_memfn_of_sharedptr_object( );
    test_dispatch_memfn_of_sharedptr_object( );
    test_execute_const_memfn_of_sharedptr_object( );
}
