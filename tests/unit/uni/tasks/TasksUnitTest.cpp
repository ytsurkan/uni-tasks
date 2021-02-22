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
        uni::Task< decltype( f ), int, int > task( f, 2, 3 );
        task( );
    }
    {
        auto task = uni::create_task( f, 2, 3 );
        task( );
    }
}

TEST_F( TasksTest, test_create_task_from_lambda )
{
    const auto f = []( int32_t a, int32_t b ) {
        const int value = a + b;
        const int expected = 2 + 3;
        ASSERT_EQ( value, expected );
    };

    const auto task = uni::create_task_ptr( f, 2, 3 );
    ( *task )( );
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
    const int v = 2;
    const auto task = uni::create_task_ptr( sum, 1, v, result );
    ( *task )( );

    const int32_t expected = ( 1 + 2 );
    ASSERT_EQ( expected, *result );
}

TEST_F( TasksTest, test_create_task_for_overloaded_memfn )
{
    FakeWorker foo;
    /// For overloaded functions it's not clear which function should be taken,
    /// therefore the function prototype of overloaded function should be explicitly specified.
    auto task = uni::create_task(
        &foo,
        uni::MemFnPtrWrapper< void ( FakeWorker::* )( int, int ) >( &FakeWorker::do_1 ),
        42,
        1 );
    task( );

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

        ( *task )( );

        const int expected = ( 42 + 1 );
        ASSERT_EQ( expected, foo.m_result );
    }
    {
        const int expected = 42;
        FakeWorker foo;
        const auto task = uni::create_task_ptr( &foo, &FakeWorker::do_2, expected );

        ( *task )( );

        ASSERT_EQ( expected, foo.m_result );
    }
}

TEST_F( TasksTest, test_create_task_for_const_memfn )
{
    FakeWorker foo;
    const int a = 42;
    const auto task = uni::create_task_ptr( &foo, &FakeWorker::do_3, a );
    ( *task )( );

    const int expected = a;
    ASSERT_EQ( expected, foo.m_result );
}

TEST_F( TasksTest, test_create_task_for_memfn_of_sharedptr_object )
{
    const auto sp = std::make_shared< FakeWorker >( );
    const auto task = uni::create_task_ptr( sp, &FakeWorker::do_2, 42 );
    ( *task )( );

    const int expected = 42;
    ASSERT_EQ( expected, sp->m_result );
}

TEST_F( TasksTest, test_create_task_for_const_memfn_of_sharedptr_object )
{
    const auto sp = std::make_shared< FakeWorker >( );
    const auto task = uni::create_task_ptr( sp, &FakeWorker::do_2, 42 );
    ( *task )( );

    const int expected = 42;
    ASSERT_EQ( expected, sp->m_result );
}

TEST_F( TasksTest, test_create_array_of_tasks_and_call )
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
        ( *task_ptr )( );
    }
    const int expected = 10 + 10 + 10;
    ASSERT_EQ( expected, accumulator );
}
