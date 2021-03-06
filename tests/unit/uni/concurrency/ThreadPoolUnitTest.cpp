#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "uni/concurrency/ThreadPool.hpp"
#include "uni/tasks/TaskComparators.hpp"
#include "uni/tasks/TaskFactory.hpp"

namespace unit
{
namespace uni
{
namespace concurrency
{
class ThreadPoolTest : public ::testing::Test
{
};

}  // namespace concurrency
}  // namespace uni
}  // namespace unit

using unit::uni::concurrency::ThreadPoolTest;

TEST_F( ThreadPoolTest, test_thread_pool_add_and_execute_lambda_task_ptr )
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

    for ( size_t i = 0; i < N; ++i )
    {
        const size_t expected = i;
        ASSERT_EQ( task_done_pop[ i ].get( ), expected );
    }

    thread_pool.stop( );
}

namespace
{
void
sum( int32_t first, int32_t second, std::shared_ptr< int32_t > sum )
{
    *sum = first + second;
}

struct FakeWorker
{
    void
    do_1( int32_t value )
    {
        m_result = value;
    }

    void
    do_2( int32_t value )
    {
        m_result2 = value;
    }

    void
    do_3( int32_t value, std::promise< size_t > promise )
    {
        m_result = value;
        promise.set_value( m_result );
    }

    mutable int m_result{};
    mutable int m_result2{};
};

}  // namespace

TEST_F( ThreadPoolTest, test_thread_pool_add_and_execute_memfn_task_ptr )
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

    FakeWorker foo[ N ];
    for ( std::size_t i = 0; i < N; ++i )
    {
        auto task = uni::create_task_ptr( &( foo[ i ] ),
                                          &FakeWorker::do_3,
                                          static_cast< int32_t >( i ),
                                          std::move( task_done_push[ i ] ) );
        thread_pool.add_task( std::move( task ) );
    }

    for ( size_t i = 0; i < N; ++i )
    {
        const size_t expected = i;
        ASSERT_EQ( task_done_pop[ i ].get( ), expected );
    }

    thread_pool.stop( );
}

TEST_F( ThreadPoolTest, test_thread_pool_of_tasks_with_lambda )
{
    std::string some_value(
        "afadfdfdsgffdsgfsdgfsdgsdfgsdfghfdsgsdhgfsdhdsfhsdfhsdhsdhsdhshshsghsdhsdhsd" );
    const auto f = [some_value]( int32_t a, int32_t b ) {
        const int value = a + b;
        const int expected = 2 + 3;
        ASSERT_EQ( value, expected );
    };

    auto task = uni::create_task( f, 2, 3 );

    uni::ThreadPool< decltype( task ), uni::TaskCmp > thread_pool;
    thread_pool.start( );
    thread_pool.add_task( std::move( task ) );
    thread_pool.stop( );
}

TEST_F( ThreadPoolTest, test_thread_pool_of_tasks_with_function )
{
    auto result1 = std::make_shared< int32_t >( );
    auto result2 = std::make_shared< int32_t >( );
    const int32_t v1 = 1;
    auto task1 = uni::create_task( sum, v1, 2, result1 );
    auto task2 = uni::create_task( sum, 1, 2, result2 );

    uni::ThreadPool< decltype( task1 ), uni::TaskCmp > thread_pool;
    thread_pool.start( );
    thread_pool.add_task( std::move( task1 ) );
    thread_pool.add_task( std::move( task2 ) );
    thread_pool.stop( );

    const int32_t expected = ( 1 + 2 );
    ASSERT_EQ( expected, *result1 );
    ASSERT_EQ( expected, *result2 );
}

TEST_F( ThreadPoolTest, test_thread_pool_of_tasks_with_memfn )
{
    const int32_t expected1 = 42;
    const int32_t expected2 = 1;
    FakeWorker foo;

    auto task1 = uni::create_task( &foo, &FakeWorker::do_1, expected1 );
    auto task2 = uni::create_task( &foo, &FakeWorker::do_2, 1 );
    uni::ThreadPool< decltype( task1 ), uni::TaskCmp > thread_pool;
    thread_pool.start( );
    thread_pool.add_task( std::move( task1 ) );
    thread_pool.add_task( std::move( task2 ) );

    thread_pool.stop( );

    ASSERT_EQ( expected1, foo.m_result );
    ASSERT_EQ( expected2, foo.m_result2 );
}
