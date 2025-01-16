enable_testing()
include(GoogleTest)

add_executable(boundary_test boundary_test.cpp)
target_link_libraries(boundary_test PRIVATE coordinate)
target_link_libraries(boundary_test PRIVATE GTest::gtest_main GTest::gmock_main)

add_executable(inner_test inner_test.cpp)
target_link_libraries(inner_test PRIVATE coordinate
)
target_link_libraries(inner_test PRIVATE GTest::gtest_main GTest::gmock_main)

add_executable(fish_test fish_test.cpp)
target_link_libraries(fish_test PRIVATE fish)
target_link_libraries(fish_test PRIVATE GTest::gtest_main GTest::gmock_main)

add_executable(io_test io_test.cpp)
target_link_libraries(io_test PRIVATE io)
target_link_libraries(io_test PRIVATE GTest::gtest_main GTest::gmock_main)

add_executable(eom_test eom_test.cpp)
target_link_libraries(eom_test PRIVATE eom fish coordinate)
target_link_libraries(eom_test PRIVATE GTest::gtest_main GTest::gmock_main)

add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test coordinate)
target_link_libraries(vector_test GTest::gtest_main GTest::gmock_main)

# Set the clang-tidy checks
set(TEST_TARGETS boundary_test inner_test fish_test io_test eom_test vector_test)
get_target_property(OPTION_TIDY project_options CXX_CLANG_TIDY)
set_target_properties(${TEST_TARGETS} PROPERTIES CXX_CLANG_TIDY "${OPTION_TIDY}")

foreach(test ${TEST_TARGETS})
    gtest_discover_tests(${test})
endforeach()
