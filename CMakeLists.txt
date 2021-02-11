project( uni-tasks )

set( HEADERS
    "public/uni/Types.hpp"
    "public/uni/common/TaskDispatcher.hpp"
    "public/uni/common/Request.hpp"
    "public/uni/common/RequestFactory.hpp"
    "public/uni/common/Runtime.hpp"
    "public/uni/common/PollEngine.hpp"
    "public/uni/concurrency/Queue.hpp"
    "public/uni/concurrency/ThreadPool.hpp"
    "public/uni/concurrency/ThreadPoolImpl.hpp"
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
    "tests/integration/uni/common/DemoComponent.hpp"
    "tests/integration/uni/common/DemoComponent.cpp"
    "tests/integration/uni/common/DemoComponentListener.hpp"
    "tests/integration/uni/common/DemoComponentIntegrationTests.hpp"
    "tests/integration/uni/common/DemoComponentIntegrationTests.cpp"
)

#treat_all_warnings_as_errors()

add_library( ${PROJECT_NAME} SHARED
    ${HEADERS}
    ${SOURCES}
#    ${TESTS_SOURCES}
)

#add_executable( ${PROJECT_NAME}
#    ${HEADERS}
#    ${SOURCES}
#    ${TESTS_SOURCES}
#)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/uni-tasks/public
    PRIVATE
        ${SOURCE_DIR}/uni-tasks/src
#        ${SOURCE_DIR}/uni-tasks/tests/unit
#        ${SOURCE_DIR}/uni-tasks/tests/integration
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The trailing slash is important
install( DIRECTORY ${CMAKE_INSTALL_INCLUDEDIR}/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_subdirectory( tests )

#if( CODE_COVERAGE )
#    generate_coverage( ${PROJECT_NAME} ${PROJECT_NAME}-integrationtests)

#    install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${COVERAGE_OUTPUT_DIR_NAME}/
#             DESTINATION ${CMAKE_INSTALL_DOCDIR}/uni/common
#    )
#endif( )
