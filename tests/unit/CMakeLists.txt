project( uni-tasks-unittests )

set( HEADERS
    ""
)

set( SOURCES
    "uni/tasks/TasksUnitTest.cpp"
    "uni/concurrency/ThreadPoolUnitTest.cpp"
    "uni/common/TaskDispatcherUnitTest.cpp"
)

add_executable( ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/public
    PRIVATE
        ${SOURCE_DIR}/tests/unit/
        ${SOURCE_DIR}/external/gtest/googletest/
        ${SOURCE_DIR}/external/gtest/googlemock/
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        uni-tasks
        gtest
        gtest_main
        gmock
)
