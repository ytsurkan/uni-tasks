project( uni-tasks )

set( HEADERS
    "public/uni/Types.hpp"
    "public/uni/common/TaskDispatcher.hpp"
    "public/uni/common/Request.hpp"
    "public/uni/common/RequestFactory.hpp"
    "public/uni/common/Runtime.hpp"
    "public/uni/common/PollEngine.hpp"
    "public/uni/tasks/ITask.hpp"
    "public/uni/tasks/TaskBase.hpp"
    "public/uni/tasks/Task.hpp"
    "public/uni/tasks/MemFnTask.hpp"
    "public/uni/tasks/TaskFactory.hpp"
    "public/uni/tasks/TaskComparators.hpp"
    "public/uni/utils/Utils.hpp"
)

set( SOURCES
    "app/main.cpp"
    "src/uni/common/Request.cpp"
    "src/uni/common/Runtime.cpp"
    "src/uni/common/PollEngine.cpp"
    "src/uni/concurrency/Queue.hpp"
    "src/uni/concurrency/ThreadPool.hpp"
    "src/uni/concurrency/ThreadPoolImpl.hpp"
    "src/uni/tasks/TaskBase.cpp"
    "src/uni/utils/Utils.cpp"
)

set( TESTS_SOURCES
    "tests/unit/uni/tasks/TasksUnitTests.hpp"
    "tests/unit/uni/tasks/TasksUnitTests.cpp"
    "tests/unit/uni/concurrency/ThreadPoolUnitTests.hpp"
    "tests/unit/uni/concurrency/ThreadPoolUnitTests.cpp"
    "tests/unit/uni/common/TaskDispatcherUnitTests.hpp"
    "tests/unit/uni/common/TaskDispatcherUnitTests.cpp"
    "tests/integration/uni/common/PollEngineIntegrationTests.hpp"
    "tests/integration/uni/common/PollEngineIntegrationTests.cpp"
    "tests/integration/uni/common/DemoComponent.hpp"
    "tests/integration/uni/common/DemoComponent.cpp"
    "tests/integration/uni/common/DemoComponentListener.hpp"
    "tests/integration/uni/common/DemoComponentIntegrationTests.hpp"
    "tests/integration/uni/common/DemoComponentIntegrationTests.cpp"
)

#treat_all_warnings_as_errors()

add_executable( ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    ${TESTS_SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/uni-tasks/public
    PRIVATE
        ${SOURCE_DIR}/uni-tasks/src
        ${SOURCE_DIR}/uni-tasks/tests/unit
        ${SOURCE_DIR}/uni-tasks/tests/integration
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        #uni-common
)

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION usr/lib
    LIBRARY DESTINATION usr/lib
    PUBLIC_HEADER DESTINATION usr/include
    RUNTIME DESTINATION usr/bin
)
