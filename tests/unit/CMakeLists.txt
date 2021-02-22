project( uni-tasks-unittests )

set( HEADERS
    ""
)

set( SOURCES
    "uni/tasks/TasksUnitTest.cpp"
    "uni/concurrency/ThreadPoolUnitTests.cpp"
)

# treat_all_warnings_as_errors()

add_executable( ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/uni-tasks/public
    PRIVATE
        ${SOURCE_DIR}/uni-tasks/tests/unit/
        ${SOURCE_DIR}/external/gtest/googletest/
        ${SOURCE_DIR}/external/gtest/googlemock/
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        uni-tasks
        gtest
        gtest-main
        gmock
)

if( CODE_COVERAGE )
    add_custom_target( ${PROJECT_NAME}_run ALL
                        ${PROJECT_NAME}
                        DEPENDS ${PROJECT_NAME}
                        VERBATIM
                        USES_TERMINAL )
endif( )

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

