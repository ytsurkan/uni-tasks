project( uni-tasks )

set( HEADERS
    "public/uni/Types.hpp"
    "public/uni/common/TaskDispatcher.hpp"
    "public/uni/common/TaskDispatcherBasic.hpp"
    "public/uni/common/TaskDispatcherBasic.inl"
    "public/uni/common/TaskDispatcher.inl"
    "public/uni/common/TaskDispatcherImpl.hpp"
    "public/uni/common/Request.hpp"
    "public/uni/common/RequestCallbacks.hpp"
    "public/uni/common/RequestFactory.hpp"
    "public/uni/common/Runtime.hpp"
    "public/uni/common/RuntimeFactory.hpp"
    "public/uni/common/PollEngine.hpp"
    "public/uni/concurrency/Queue.hpp"
    "public/uni/concurrency/ThreadPool.hpp"
    "public/uni/concurrency/ThreadPoolImpl.hpp"
    "public/uni/tasks/ITask.hpp"
    "public/uni/tasks/Task.hpp"
    "public/uni/tasks/TaskImpl.hpp"
    "public/uni/tasks/TaskBaseImpl.hpp"
    "public/uni/tasks/MemberFnTask.hpp"
    "public/uni/tasks/MemberFnTaskImpl.hpp"
    "public/uni/tasks/TaskFactory.hpp"
    "public/uni/tasks/TaskFactory.inl"
    "public/uni/tasks/TaskFactoryBasic.hpp"
    "public/uni/tasks/TaskFactoryBasic.inl"
    "public/uni/tasks/TaskFactoryImpl.hpp"
    "public/uni/tasks/TaskComparators.hpp"
    "public/uni/tasks/TaskUtils.hpp"
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
    "src/uni/common/PollEngine.cpp"
    "src/uni/common/TaskDispatcher.cpp"
    "src/uni/common/TaskDispatcherBasic.cpp"
    "src/uni/common/TaskDispatcherImpl.cpp"
    "src/uni/common/RequestFactory.cpp"
    "src/uni/common/RuntimeFactory.cpp"
    "src/uni/tasks/TaskBaseImpl.cpp"
    "src/uni/utils/Utils.cpp"
)

#treat_all_warnings_as_errors()

add_library( ${PROJECT_NAME} STATIC
    ${HEADERS}
    ${SOURCES}
)


target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/uni-tasks/public
    PRIVATE
        ${SOURCE_DIR}/uni-tasks/src
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
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
