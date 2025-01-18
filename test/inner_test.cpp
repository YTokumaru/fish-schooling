#include "coordinate.hpp"
#include <array>
#include <cmath>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <numbers>
#include <vector>

using namespace testing;

TEST(InnerTest, RadiusZero)
{
  const double radius = 0.0;
  auto result = getInnerCells(radius);
  EXPECT_TRUE(result.empty());
}

TEST(InnerTest, RadiusLessThanRootThree)
{
  const double radius = std::sqrt(2.9);
  auto result = getInnerCells(radius);
  EXPECT_TRUE(result.empty());
}

TEST(InnerTest, RadiusRootThree)
{
  const double radius = std::numbers::sqrt3;
  auto result = getInnerCells(radius);
  const std::vector<std::array<int, 3>> expected = { { 0, 0, 0 } };
  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerTest, RadiusTwo)
{
  const double radius = 2.0;
  auto result = getInnerCells(radius);
  const std::vector<std::array<int, 3>> expected = { { 0, 0, 0 } };
  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerTest, RadiusRootSix)
{
  const double radius = std::sqrt(6);
  auto result = getInnerCells(radius);
  const std::vector<std::array<int, 3>> expected = {
    { 0, 0, 0 },
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
    { -1, 0, 0 },
    { 0, -1, 0 },
    { 0, 0, -1 },
  };
  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerBetweenTest, RadiusZero)
{
  const double radius1 = 0.0;
  const double radius2 = 0.0;
  auto result = getInnerBetween(radius1, radius2);
  EXPECT_TRUE(result.empty());
}

TEST(InnerBetweenTest, OneRadiusZero)
{
  const double radius1 = 0.0;
  const double radius2 = 5.0;
  auto result = getInnerBetween(radius1, radius2);
  auto expected = getInnerCells(radius2);
  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerBetweenTest, RadiusLessThanOrEqualToPointEight)
{
  const double radius1 = std::sqrt(0.75);
  const double radius2 = std::sqrt(0.75);
  auto result = getInnerBetween(radius1, radius2);
  EXPECT_TRUE(result.empty());
}

TEST(InnerBetweenTest, SameRadius)
{
  const double radius = 2.5;
  auto result = getInnerBetween(radius, radius);
  EXPECT_TRUE(result.empty());
}

TEST(InnerBetweenTest, InnerIsEmpty)
{
  const double radius1 = 0.5;
  const double radius2 = 2.0;
  auto result = getInnerBetween(radius1, radius2);
  EXPECT_TRUE(result.empty());
}
