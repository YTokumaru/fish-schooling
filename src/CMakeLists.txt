add_executable(fish_school main.cpp)
target_link_libraries(fish_school PRIVATE project_warnings)
target_link_libraries(fish_school PRIVATE fish coordinate eom)

add_library(coordinate coordinate.cpp)
target_include_directories(coordinate PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_library(fish fish.cpp)
target_include_directories(fish PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_library(eom eom.cpp)
target_include_directories(eom PUBLIC "${PROJECT_SOURCE_DIR}/include")