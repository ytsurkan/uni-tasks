#include "uni/concurrency/ThreadPool.hpp"
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

}  // namespace

void
test_thread_pool_add_and_execute_lambda_task( )
{
    constexpr auto N = std::size_t{7};
    std::future< std::size_t > task_done_pop[ N ];
    std::promise< std::size_t > task_done_push[ N ];

    for ( std::size_t i = 0; i < N; ++i )
    {
        task_done_pop[ i ] = task_done_push[ i ].get_future( );
    }

    uni::ThreadPool< std::unique_ptr< uni::ITask >, uni::TaskPtrCmp > thread_pool;
    thread_pool.start( );

    for ( std::size_t i = 0; i < N; ++i )
    {
        auto task = [promise = &task_done_push[ i ], i]( ) { promise->set_value( i ); };
        thread_pool.add_task( uni::create_task_ptr( std::move( task ) ) );
    }

    for ( std::size_t i = 0; i < N; ++i )
    {
        ASSERT( task_done_pop[ i ].get( ) == i );
    }

    thread_pool.stop( );
}

void
test_thread_pool_add_and_execute_memfn_task( )
{
    constexpr auto N = std::size_t{7};
    std::future< std::size_t > task_done_pop[ N ];
    std::promise< std::size_t > task_done_push[ N ];

    for ( std::size_t i = 0; i < N; ++i )
    {
        task_done_pop[ i ] = task_done_push[ i ].get_future( );
    }

    uni::ThreadPool< std::unique_ptr< uni::ITask >, uni::TaskPtrCmp > thread_pool;
    thread_pool.start( );

    Listener foo[ N ];
    for ( std::size_t i = 0; i < N; ++i )
    {
        auto task = uni::create_task_ptr(
            &( foo[ i ] ),
            uni::MemFnPtrWrapper< void ( Listener::* )( int, int, std::promise< size_t > ) >(
                &Listener::on_result ),
            static_cast< int >( i ),
            static_cast< int >( i ),
            std::move( task_done_push[ i ] ) );
        thread_pool.add_task( std::move( task ) );
    }

    for ( std::size_t i = 0; i < N; ++i )
    {
        ASSERT( task_done_pop[ i ].get( ) == ( i + i ) );
    }

    thread_pool.stop( );
}

void
test_thread_pool_of_value_tasks_with_lambda( )
{
    std::string some_value(
        "afadfdfdsgffdsgfsdgfsdgsdfgsdfghfdsgsdhgfsdhdsfhsdfhsdhsdhsdhshshsghsdhsdhsd" );
    const auto f = [some_value]( int a, int b ) {
        const int r = a + b;
        ASSERT_EQ( r, ( 2 + 3 ) );
        return r;
    };
    auto task = uni::create_task( f, 2, 3 );

    uni::ThreadPool< decltype( task ), uni::TaskCmp > thread_pool;
    thread_pool.start( );
    thread_pool.add_task( std::move( task ) );
    thread_pool.stop( );
}

void
test_thread_pool_of_value_tasks_with_function( )
{
    int v1 = 1;
    auto task = uni::create_task( add, v1, 2 );

    uni::ThreadPool< decltype( task ), uni::TaskCmp > thread_pool;
    thread_pool.start( );
    thread_pool.add_task( std::move( task ) );
    thread_pool.stop( );
}

void
test_thread_pool_of_value_tasks_with_memfn( )
{
    const int expected = 42;
    Listener foo;

    auto task = uni::create_task( &foo, &Listener::on_calculation_done, expected );
    uni::ThreadPool< decltype( task ), uni::TaskCmp > thread_pool;
    thread_pool.start( );
    thread_pool.add_task( std::move( task ) );

    thread_pool.stop( );

    ASSERT_EQ( expected, foo.m_result );
}

void
thread_pool_tests( )
{
    test_thread_pool_of_value_tasks_with_lambda( );
    test_thread_pool_of_value_tasks_with_function( );
    test_thread_pool_of_value_tasks_with_memfn( );
    test_thread_pool_add_and_execute_lambda_task( );
    test_thread_pool_add_and_execute_memfn_task( );
}
