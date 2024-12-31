# Define options for different build types
macro(project_setup_options)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Compiler flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

        option(PROJECT_ENABLE_CLANG_TIDY "Enable clang-tidy checks" ON)
        option(PROJECT_ENABLE_CPPCHECK "Enable cppcheck checks" ON)
        option(PROJECT_ENABLE_WARNINGS "Enable compiler warnings" ON)
        option(PROJECT_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
        option(PROJECT_BUILD_TESTS "Build tests" ON)
        option(PROJECT_COMPILER_WARNINGS "Enable compiler warnings" ON)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    
    add_library(project_warnings INTERFACE)


    # Set the options
    include(cmake/StaticAnalyzers.cmake)
    if(PROJECT_ENABLE_CPPCHECK)
        message(STATUS "Enabling cppcheck")
        project_enable_cppcheck(${PROJECT_ENABLE_WARNINGS_AS_ERRORS} "")
    endif(PROJECT_ENABLE_CPPCHECK)

    if(PROJECT_ENABLE_CLANG_TIDY)
        message(STATUS "Enabling clang-tidy")
        project_enable_clang_tidy(${PROJECT_ENABLE_WARNINGS_AS_ERRORS})
    endif(PROJECT_ENABLE_CLANG_TIDY)
    
    if(PROJECT_BUILD_TESTS)
        message(STATUS "Enabling tests")
        include(FetchContent)
        FetchContent_Declare(
          googletest
          URL https://github.com/google/googletest/archive/refs/tags/v1.12.0.zip
        )
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
        include(CTest)
        add_subdirectory(test)
    endif(PROJECT_BUILD_TESTS)

    if(PROJECT_COMPILER_WARNINGS)
        message(STATUS "Enabling compiler warnings")
        include(cmake/CompilerWarnings.cmake)
        project_enable_warnings(project_warnings ${PROJECT_ENABLE_WARNINGS_AS_ERRORS} "" "" "" "")
    endif(PROJECT_COMPILER_WARNINGS)
endmacro(project_setup_options)