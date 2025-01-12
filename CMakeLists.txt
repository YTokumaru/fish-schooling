cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(EXPORT_COMPILE_COMMANDS ON)

project(
  fish-school
  VERSION 0.0.1
  DESCRIPTION "Simulator of fish schooling"
  LANGUAGES CXX C)

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_MakeAvailable(yaml-cpp)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message(STATUS "OpenMP found")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include(ProjectOptions.cmake)
project_setup_options()

add_subdirectory(src)

# Copy the config.yaml file to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/config.yaml DESTINATION ${CMAKE_BINARY_DIR}/src)

install(TARGETS fish_school DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/config.yaml DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/create_movie.sh DESTINATION .
PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE)