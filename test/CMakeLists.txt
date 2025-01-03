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

add_executable(
    fish_test
    fish_test.cpp
)
target_link_libraries(
    fish_test
    fish
)
target_link_libraries(
    fish_test
    GTest::gtest_main
    GTest::gmock_main
)

add_executable(
    io_test
    io_test.cpp
)
target_link_libraries(
    io_test
    io
)
target_link_libraries(
    io_test
    GTest::gtest_main
    GTest::gmock_main
)

add_executable(
    eom_test
    eom_test.cpp
)
target_link_libraries(
    eom_test
    eom
    fish
    coordinate
)
target_link_libraries(
    eom_test
    GTest::gtest_main
    GTest::gmock_main
)

add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test coordinate)
target_link_libraries(vector_test GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(boundary_test)
gtest_discover_tests(inner_test)
gtest_discover_tests(fish_test)
gtest_discover_tests(io_test)
gtest_discover_tests(vector_test)
gtest_discover_tests(eom_test)

enable_testing()