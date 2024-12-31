#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "coordinate.hpp"

using namespace testing;

TEST(BoundaryTest, RadiusZero) {
    double radius = 0.0;
    auto result = getBoundaryCells(radius);
    EXPECT_TRUE(result.empty());
}

TEST(BoundaryTest, RadiusLessThanOrEqualToPointFive) {
    double radius = 0.5;
    auto result = getBoundaryCells(radius);
    ASSERT_EQ(result.size(), 1);
    ASSERT_THAT(result, UnorderedElementsAreArray(std::vector<std::array<int, 3>>{{0, 0, 0}}));
}

TEST(BoundaryTest, RadiusLittleGreaterThanPointFive) {
    double radius = 0.8;
    auto result = getBoundaryCells(radius);
    std::vector<std::array<int, 3>> expected = {
        {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}, {0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {0, 0, 0},
        {1,1,0}, {1, -1, 0}, {-1, 1, 0}, {-1, -1, 0}, 
        {1, 0, 1}, {-1, 0, 1}, {0, 1, 1}, {0, -1, 1},
        {1, 0, -1}, {-1, 0, -1}, {0, 1, -1}, {0, -1, -1},
    };
    EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(BoundaryTest, RadiusGreaterThanOne) {
    double radius = 1.5;
    auto result = getBoundaryCells(radius);
    std::vector<std::array<int, 3>> expected = {
        {-1, -1, 1}, {-1, 0, 1}, {-1, 1, 1}, {0, -1, 1}, {0, 0, 1}, {0, 1, 1}, {1, -1, 1}, {1, 0, 1}, {1, 1, 1},
        {-1, -1, 0}, {-1, 0, 0}, {-1, 1, 0}, {0, -1, 0}, {0, 1, 0}, {1, -1, 0}, {1, 0, 0}, {1, 1, 0},
        {-1, -1, -1}, {-1, 0, -1}, {-1, 1, -1}, {0, -1, -1}, {0, 0, -1}, {0, 1, -1}, {1, -1, -1}, {1, 0, -1}, {1, 1, -1}
    };
    EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(BoundaryBetweenTest, RadiusZero) {
    double radius1 = 0.0;
    double radius2 = 0.0;
    auto result = getBoundaryBetween(radius1, radius2);
    EXPECT_TRUE(result.empty());
}

TEST(BoundaryBetweenTest, RadiusLessThanOrEqualToPointFive) {
    double radius1 = 0.5;
    double radius2 = 0.5;
    auto result = getBoundaryBetween(radius1, radius2);
    ASSERT_THAT(result, UnorderedElementsAreArray(std::vector<std::array<int, 3>>{{0, 0, 0}}));
}

TEST(BoundaryBetweenTest, SameRadius)
{
    double radius = 2.5;
    auto result = getBoundaryBetween(radius, radius);

    auto other_result = getBoundaryCells(radius);

    EXPECT_THAT(result, UnorderedElementsAreArray(other_result));
}

TEST(BoundaryBetweenTest, RadiusFlipped)
{
    double radius1 = 4.5;
    double radius2 = 1.5;
    auto result1 = getBoundaryBetween(radius1, radius2);
    auto result2 = getBoundaryBetween(radius2, radius1);

    EXPECT_THAT(result1, UnorderedElementsAreArray(result2));
}