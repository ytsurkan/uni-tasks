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
