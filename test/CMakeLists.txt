add_executable(
    boundary_test
    boundary_test.cpp
)
target_link_libraries(
    boundary_test
    coordinate
)
target_link_libraries(
    boundary_test
    GTest::gtest_main
    GTest::gmock_main
)

add_executable(
    inner_test
    inner_test.cpp
)
target_link_libraries(
    inner_test
    coordinate
)
target_link_libraries(
    inner_test
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(boundary_test)
gtest_discover_tests(inner_test)

enable_testing()