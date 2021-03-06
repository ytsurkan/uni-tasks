include(FetchContent)

macro(fetch_googletest)
    FetchContent_Declare(
       googletest
       GIT_REPOSITORY https://github.com/google/googletest.git
       GIT_TAG        release-1.10.0
    )

    FetchContent_GetProperties(googletest)
endmacro()
