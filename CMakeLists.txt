cmake_minimum_required( VERSION 3.11 )

project( uni-tasks )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(bootstrap)
include(FetchContent)

include(CMakePrintHelpers)

uni_cmake_set_cxxstd(17)

set( SOURCE_DIR "${CMAKE_SOURCE_DIR}" )
set( BUILD_DIR "${CMAKE_BINARY_DIR}" )
set( INSTALL_DIR "${BUILD_DIR}/install" )

set( CMAKE_INSTALL_BINDIR bin )
set( CMAKE_INSTALL_DOCDIR doc )
set( CMAKE_INSTALL_INCLUDEDIR include )
set( CMAKE_INSTALL_LIBDIR lib )

set( CMAKE_INSTALL_PREFIX ${INSTALL_DIR} )
set( CMAKE_INSTALL_RPATH ${INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR} )

set( HEADERS
    "public/uni/Types.hpp"
    "public/uni/common/TaskDispatcher.hpp"
    "public/uni/common/TaskDispatcherBasic.hpp"
    "public/uni/common/details/TaskDispatcherBasic.inl"
    "public/uni/common/details/TaskDispatcher.inl"
    "public/uni/common/details/TaskDispatcherImpl.hpp"
    "public/uni/common/Request.hpp"
    "public/uni/common/RequestCallbacks.hpp"
    "public/uni/common/RequestFactory.hpp"
    "public/uni/common/Runtime.hpp"
    "public/uni/common/RuntimeFactory.hpp"
    "public/uni/common/PollEngine.hpp"
    "public/uni/concurrency/Queue.hpp"
    "public/uni/concurrency/ThreadPool.hpp"
    "public/uni/concurrency/details/ThreadPoolImpl.hpp"
    "public/uni/tasks/ITask.hpp"
    "public/uni/tasks/details/Task.hpp"
    "public/uni/tasks/details/TaskImpl.hpp"
    "public/uni/tasks/details/TaskBaseImpl.hpp"
    "public/uni/tasks/details/MemberFnTask.hpp"
    "public/uni/tasks/details/MemberFnTaskImpl.hpp"
    "public/uni/tasks/TaskFactory.hpp"
    "public/uni/tasks/details/TaskFactory.inl"
    "public/uni/tasks/TaskFactoryBasic.hpp"
    "public/uni/tasks/details/TaskFactoryBasic.inl"
    "public/uni/tasks/details/TaskFactoryImpl.hpp"
    "public/uni/tasks/TaskComparators.hpp"
    "public/uni/tasks/details/TaskUtils.hpp"
    "public/uni/utils/Utils.hpp"
    "public/uni/utils/TypeTraits.hpp"
)

set( SOURCES
    "src/uni/common/Request.cpp"
    "src/uni/common/RequestImpl.hpp"
    "src/uni/common/RequestImpl.cpp"
    "src/uni/common/RuntimeImpl.hpp"
    "src/uni/common/RuntimeImpl.cpp"
    "src/uni/common/Runtime.cpp"
    "src/uni/common/PollEngineImpl.hpp"
    "src/uni/common/PollEngineImpl.cpp"
    "src/uni/common/PollEngine.cpp"
    "src/uni/common/TaskDispatcher.cpp"
    "src/uni/common/TaskDispatcherBasic.cpp"
    "src/uni/common/TaskDispatcherImpl.cpp"
    "src/uni/common/RequestFactory.cpp"
    "src/uni/common/RuntimeFactory.cpp"
    "src/uni/tasks/TaskBaseImpl.cpp"
    "src/uni/utils/Utils.cpp"
)

add_library( ${PROJECT_NAME} STATIC
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/public
    PRIVATE
        ${SOURCE_DIR}/src
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.10.0
    )

    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
      FetchContent_Populate(googletest)
      add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

    cmake_print_variables(googletest_SOURCE_DIR SOURCE_DIR BUILD_DIR)

    add_subdirectory( tests )
endif()


install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The trailing slash is important
install(DIRECTORY ${SOURCE_DIR}/public/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp"
)
