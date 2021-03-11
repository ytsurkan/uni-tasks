#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "uni/tasks/TaskFactory.hpp"

namespace unit
{
namespace uni
{
namespace common
{
class TasksTest : public ::testing::Test
{
};

}  // namespace common
}  // namespace uni
}  // namespace unit

using unit::uni::common::TasksTest;

TEST_F( TasksTest, test_task_call )
{
    std::string ss(
        "afadfdfdsgffdsgfsdgfsdgsdfgsdfghfdsgsdhgfsdhdsfhsdfhsdhsdhsdhshshsghsdhsdhsd" );
    const auto f = [ss]( int32_t a, int32_t b ) {
        const int value = a + b;
        const int expected = 2 + 3;
        ASSERT_EQ( value, expected );
    };
    {
        uni::ITask&& task = uni::Task< decltype( f ), int, int >( f, 2, 3 );
        task.run( );
    }
    {
        uni::ITask&& task = uni::create_task( f, 2, 3 );
        task.run( );
    }
}

TEST_F( TasksTest, test_create_task_from_lambda )
{
    const auto f = []( int32_t a, int32_t b ) {
        const int32_t value = a + b;
        const int32_t expected = 2 + 3;
        ASSERT_EQ( value, expected );
    };

    const auto task = uni::create_task_ptr( f, 2, 3 );
    task->run( );
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
    static constexpr size_t s_magic_number = 13;

    void
    do_1( int value1, int value2 )
    {
        m_result = value1 + value2;
    }

    void
    do_1( int value1, int value2 ) const
    {
        m_result = value1 + value2 + s_magic_number;
    }

    void
    do_2( int value )
    {
        m_result = value;
    }

    void
    do_3( int value ) const
    {
        m_result = value;
    }

    mutable int m_result{};
};

}  // namespace

TEST_F( TasksTest, test_create_task_from_function )
{
    auto result = std::make_shared< int32_t >( );
    const int32_t v = 2;
    const auto task = uni::create_task_ptr( sum, 1, v, result );
    task->run( );

    const int32_t expected = ( 1 + 2 );
    ASSERT_EQ( expected, *result );
}

TEST_F( TasksTest, test_create_task_for_overloaded_memfn )
{
    FakeWorker foo;
    /// For overloaded functions it's not clear which function should be taken,
    /// therefore the function prototype of overloaded function should be explicitly specified.
    uni::ITask&& task = uni::create_task(
        &foo,
        uni::MemFnPtrWrapper< void ( FakeWorker::* )( int, int ) >( &FakeWorker::do_1 ),
        42,
        1 );
    task.run( );

    const int expected = ( 42 + 1 );
    ASSERT_EQ( expected, foo.m_result );
}

TEST_F( TasksTest, test_create_task_for_memfn )
{
    {
        FakeWorker foo;
        /// For overloaded functions it's not clear which function should be taken,
        /// therefore the function prototype of overloaded function should be explicitly specified.
        const auto task = uni::create_task_ptr(
            &foo,
            uni::MemFnPtrWrapper< void ( FakeWorker::* )( int, int ) >( &FakeWorker::do_1 ),
            42,
            1 );

        task->run( );

        const int expected = ( 42 + 1 );
        ASSERT_EQ( expected, foo.m_result );
    }
    {
        const int expected = 42;
        FakeWorker foo;
        const auto task = uni::create_task_ptr( &foo, &FakeWorker::do_2, expected );
        task->run( );
        ASSERT_EQ( expected, foo.m_result );
    }
}

TEST_F( TasksTest, test_create_task_for_const_memfn )
{
    FakeWorker foo;
    const int a = 42;
    const auto task = uni::create_task_ptr( &foo, &FakeWorker::do_3, a );
    task->run( );

    const int expected = a;
    ASSERT_EQ( expected, foo.m_result );
}

TEST_F( TasksTest, test_create_task_for_memfn_of_sharedptr_object )
{
    const auto sp = std::make_shared< FakeWorker >( );
    const auto task = uni::create_task_ptr( sp, &FakeWorker::do_2, 42 );
    task->run( );

    const int expected = 42;
    ASSERT_EQ( expected, sp->m_result );
}

TEST_F( TasksTest, test_create_task_for_const_memfn_of_sharedptr_object )
{
    const auto sp = std::make_shared< FakeWorker >( );
    const auto task = uni::create_task_ptr( sp, &FakeWorker::do_2, 42 );
    task->run( );

    const int expected = 42;
    ASSERT_EQ( expected, sp->m_result );
}

TEST_F( TasksTest, test_create_array_of_tasks_and_call )
{
    int32_t accumulator = 0;
    const auto f = [&accumulator]( int32_t a ) { accumulator += a; };

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
