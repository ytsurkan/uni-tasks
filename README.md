# uni-tasks

Basic implementation of task-based concurrency library.

#### Building instructions
1. Clone repository.
2. `cd` to the root of the project.
3. Create build directory and execute CMake:
  
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```
    A custom compiler can be specified.
    ```
    cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug ..
    ```
#### Code sample
    const auto f = []( int32_t a, int32_t b, int32_t expected ) {
        const int value = a + b;
        ASSERT_EQ( value, expected );
    };

    const int32_t a = 2, b = 3;
    const int32_t expected = a + b;
    auto task = uni::create_task_ptr( f, a, b, expected );

    uni::ThreadPool< std::unique_ptr< uni::ITask >, uni::TaskPtrCmp > thread_pool;
    thread_pool.start( );
    thread_pool.add_task( std::move( task ) );
    thread_pool.stop( );
    
More usage examples are covered in integration and unit tests (e.g. DemoComponentTest.cpp, PollEngineTest.cpp, ThreadPoolUnitTest.cpp).
