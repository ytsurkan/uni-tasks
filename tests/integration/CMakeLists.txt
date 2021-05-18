project( uni-tasks-integrationtests )

set( HEADERS
    "uni/common/ThreadPoolConfiguration.hpp"
)

set( SOURCES
    "uni/common/PollEngineTest.cpp"
    "uni/common/DemoComponentTest.cpp"
    "uni/common/DemoComponentImpl.cpp"
    "uni/common/DemoComponentImpl.hpp"
    "uni/common/DemoComponentListener.hpp"
)

add_executable( ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/public
    PRIVATE
        ${SOURCE_DIR}/tests/integration/
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
