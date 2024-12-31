#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "coordinate.hpp"

using namespace testing;

TEST(InnerTest, RadiusZero) {
    double radius = 0.0;
    auto result = getInnerCells(radius);
    EXPECT_TRUE(result.empty());
}

TEST(InnerTest, RadiusLessThanOrEqualToPointEight) {
    double radius = std::sqrt(0.75);
    auto result = getInnerCells(radius);
    EXPECT_TRUE(result.empty());
}

TEST(InnerTest, RadiusGreaterThanOne) {
    double radius = 1.;
    auto result = getInnerCells(radius);
    std::vector<std::array<int, 3>> expected = {
        {0,0,0}
    };
    EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerTest, RadiusQuiteLarge)
{
    double radius = 1.8;
    auto result = getInnerCells(radius);
    std::vector<std::array<int, 3>> expected = {
        {0,0,0},
        {1,0,0}, {0,1,0}, {0,0,1},
        {-1, 0, 0}, {0, -1, 0}, {0, 0, -1},
    };
    EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerBetweenTest, RadiusZero) {
    double radius1 = 0.0;
    double radius2 = 0.0;
    auto result = getInnerBetween(radius1, radius2);
    EXPECT_TRUE(result.empty());
}

TEST(InnerBetweenTest, RadiusLessThanOrEqualToPointEight) {
    double radius1 = std::sqrt(0.75);
    double radius2 = std::sqrt(0.75);
    auto result = getInnerBetween(radius1, radius2);
    EXPECT_TRUE(result.empty());
}

TEST(InnerBetweenTest, SameRadius)
{
    double radius = 2.5;
    auto result = getInnerBetween(radius, radius);
    EXPECT_TRUE(result.empty());
}

TEST(InnerBetweenTest, Radius1LessThanRadius2)
{
    double radius1 = 0.3;
    double radius2 = 1.8;
    auto result = getInnerBetween(radius1, radius2);
    std::vector<std::array<int, 3>> expected = {
        // {0,0,0},
        {1,0,0}, {0,1,0}, {0,0,1},
        {-1, 0, 0}, {0, -1, 0}, {0, 0, -1},
    };
    EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerBetweenTest, Radius1GreaterThanRadius2)
{
    double radius1 = 1.8;
    double radius2 = 0.3;
    auto result = getInnerBetween(radius1, radius2);
    std::vector<std::array<int, 3>> expected = {
        // {0,0,0},
        {1,0,0}, {0,1,0}, {0,0,1},
        {-1, 0, 0}, {0, -1, 0}, {0, 0, -1},
    };
    EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}