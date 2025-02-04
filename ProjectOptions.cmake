# Define options for different build types
macro(project_setup_options)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Compiler flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g --coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

        option(PROJECT_ENABLE_CLANG_TIDY "Enable clang-tidy checks" ON)
        option(PROJECT_ENABLE_CPPCHECK "Enable cppcheck checks" ON)
        option(PROJECT_ENABLE_WARNINGS "Enable compiler warnings" ON)
        option(PROJECT_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
        option(PROJECT_BUILD_TESTS "Build tests" ON)
        option(PROJECT_COMPILER_WARNINGS "Enable compiler warnings" ON)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Compiler flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

        option(PROJECT_ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)
        option(PROJECT_ENABLE_CPPCHECK "Enable cppcheck checks" OFF)
        option(PROJECT_ENABLE_WARNINGS "Enable compiler warnings" OFF)
        option(PROJECT_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
        option(PROJECT_BUILD_TESTS "Build tests" OFF)
        option(PROJECT_COMPILER_WARNINGS "Enable compiler warnings" OFF)
    endif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        # Compiler flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -shared-libgcc")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -shared-libgcc")

        option(PROJECT_ENABLE_CLANG_TIDY "Enable clang-tidy checks" ON)
        option(PROJECT_ENABLE_CPPCHECK "Enable cppcheck checks" ON)
        option(PROJECT_ENABLE_WARNINGS "Enable compiler warnings" ON)
        option(PROJECT_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
        option(PROJECT_BUILD_TESTS "Build tests" ON)
        option(PROJECT_COMPILER_WARNINGS "Enable compiler warnings" ON)
    endif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    
    
    add_library(project_options INTERFACE)


    # Set the options
    include(cmake/StaticAnalyzers.cmake)
    if(PROJECT_ENABLE_CPPCHECK)
        message(STATUS "Enabling cppcheck")
        project_enable_cppcheck(project_options ${PROJECT_ENABLE_WARNINGS_AS_ERRORS})
    endif(PROJECT_ENABLE_CPPCHECK)

    if(PROJECT_ENABLE_CLANG_TIDY)
        message(STATUS "Enabling clang-tidy")
        project_enable_clang_tidy(project_options ${PROJECT_ENABLE_WARNINGS_AS_ERRORS})
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
        project_enable_warnings(project_options ${PROJECT_ENABLE_WARNINGS_AS_ERRORS} "" "" "" "")
    endif(PROJECT_COMPILER_WARNINGS)
endmacro(project_setup_options)