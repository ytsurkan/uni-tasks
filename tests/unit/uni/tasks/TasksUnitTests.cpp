#include <functional>
#include <future>
#include <iostream>

#include "uni/tasks/TaskFactory.hpp"

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

#define UNUSED( x ) (void)( x );

namespace
{
int
add( int first, int second )
{
    return first + second;
}

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

template < typename F >
int
wrapper_x( int a, int b, F f )
{
    return f( a, b );
}

}  // namespace

void
test_task_call( )
{
    std::string ss(
        "afadfdfdsgffdsgfsdgfsdgsdfgsdfghfdsgsdhgfsdhdsfhsdfhsdhsdhsdhshshsghsdhsdhsd" );
    const auto f = [ss]( int a, int b ) {
        const int r = a + b;
        ASSERT_EQ( r, ( 2 + 3 ) );
        return r;
    };
    uni::Task< decltype( f ), int, int > task( f, 2, 3 );
    task( );
}

void
test_create_task_from_lambda( )
{
    const auto f = []( int a, int b ) {
        const int r = a + b;
        ASSERT_EQ( r, ( 10 + 10 ) );
        return r;
    };

    const auto task = uni::create_task_ptr( f, 10, 10 );
    ( *task )( );
}

void
test_create_task_from_function( )
{
    int result = 0;
    FunctionWrapper< decltype( add ) > fw( add, result );

    const int v = 2;
    const auto task = uni::create_task_ptr( fw, 1, v );
    task->run( );

    const int expected = ( 1 + 2 );
    ASSERT_EQ( expected, result );
}

void
test_create_task_for_overloaded_memfn( )
{
    Listener foo;
    /// For overloaded functions it's not clear which function should be taken,
    /// therefore the function prototype of overloaded function should be explicitly specified.
    auto task = uni::create_task(
        &foo,
        uni::MemFnPtrWrapper< void ( Listener::* )( int, int ) >( &Listener::on_result ),
        42,
        1 );
    task( );

    const int expected = ( 42 + 1 );
    ASSERT_EQ( expected, foo.m_result );
}

void
test_create_task_for_memfn( )
{
    {
        Listener foo;
        /// For overloaded functions it's not clear which function should be taken,
        /// therefore the function prototype of overloaded function should be explicitly specified.
        const auto task = uni::create_task_ptr(
            &foo,
            uni::MemFnPtrWrapper< void ( Listener::* )( int, int ) >( &Listener::on_result ),
            42,
            1 );

        task->run( );

        const int expected = ( 42 + 1 );
        ASSERT_EQ( expected, foo.m_result );
    }
    {
        const int expected = 42;
        Listener foo;
        const auto task = uni::create_task_ptr( &foo, &Listener::on_calculation_done, expected );

        task->run( );

        ASSERT_EQ( expected, foo.m_result );
    }
}

void
test_create_task_for_const_memfn( )
{
    Listener foo;
    const int a = 42;
    int b = 1;
    const auto task = uni::create_task_ptr( &foo, &Listener::on_result, a, b );
    task->run( );

    const int expected = ( a + b + Listener::s_magic_number );
    ASSERT_EQ( expected, foo.m_result );
}

void
test_create_task_for_memfn_of_sharedptr_object( )
{
    const auto sp = std::make_shared< Listener >( );
    const auto task = uni::create_task_ptr(
        sp,
        uni::MemFnPtrWrapper< void ( Listener::* )( int, int ) >( &Listener::on_result ),
        42,
        42 );
    task->run( );

    const int expected = ( 42 + 42 );
    ASSERT_EQ( expected, sp->m_result );
}

void
test_create_task_for_const_memfn_of_sharedptr_object( )
{
    const auto sp = std::make_shared< Listener >( );
    const auto task = uni::create_task_ptr( sp, &Listener::on_result, 42, 42 );
    task->run( );

    const int expected = ( 42 + 42 + Listener::s_magic_number );
    ASSERT_EQ( expected, sp->m_result );
}

void
test_create_array_of_tasks_and_call( )
{
    int accumulator = 0;
    const auto f = [&accumulator]( int a ) { accumulator += a; };

    std::vector< std::unique_ptr< uni::ITask > > tasks;

    auto task1 = uni::create_task_ptr( f, 10 );
    tasks.push_back( std::move( task1 ) );
    tasks.push_back( uni::create_task_ptr( f, 10 ) );
    tasks.push_back( uni::create_task_ptr( f, 10 ) );

    for ( const auto& task_ptr : tasks )
    {
        task_ptr->run( );
    }
    const int expected = 10 + 10 + 10;
    ASSERT_EQ( expected, accumulator );
}

void
task_tests( )
{
    test_task_call( );
    test_create_task_from_lambda( );
    test_create_task_from_function( );
    test_create_task_for_overloaded_memfn( );
    test_create_task_for_memfn( );
    test_create_task_for_const_memfn( );
    test_create_task_for_memfn_of_sharedptr_object( );
    test_create_task_for_memfn_of_sharedptr_object( );
    test_create_array_of_tasks_and_call( );
}
