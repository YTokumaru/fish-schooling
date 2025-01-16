add_executable(fish_school main.cpp)
target_link_libraries(fish_school PRIVATE project_options)
target_link_libraries(fish_school PRIVATE fish coordinate simulation io eom)
target_link_libraries(fish_school PRIVATE yaml-cpp::yaml-cpp argparse)
if(OpenMP_CXX_FOUND)
    target_link_libraries(fish_school PUBLIC OpenMP::OpenMP_CXX)
endif()

add_library(coordinate coordinate.cpp)
target_link_libraries(coordinate project_options)
target_include_directories(coordinate PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_library(simulation simulation.cpp)
target_include_directories(simulation PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(simulation PRIVATE coordinate project_options)

add_library(fish fish.cpp)
target_link_libraries(fish PUBLIC simulation project_options)
target_include_directories(fish PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_library(eom eom.cpp)
target_link_libraries(eom PRIVATE fish simulation project_options)
target_include_directories(eom PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_library(io io.cpp)
target_include_directories(io PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(io PRIVATE simulation project_options)
target_link_libraries(io PUBLIC yaml-cpp::yaml-cpp argparse)

# Set the clang-tidy checks
set(SRC_TARGETS fish_school coordinate simulation fish eom io)
get_target_property(OPTION_TIDY project_options CXX_CLANG_TIDY)
if(OPTION_TIDY)
    set_target_properties(${SRC_TARGETS} PROPERTIES CXX_CLANG_TIDY "${OPTION_TIDY}")
endif(OPTION_TIDY)

# Set the cppcheck checks
get_target_property(OPTION_CPPCHECK project_options CXX_CPPCHECK)
if(OPTION_CPPCHECK)
    set_target_properties(${SRC_TARGETS} PROPERTIES CXX_CPPCHECK "${OPTION_CPPCHECK}")
endif(OPTION_CPPCHECK)
