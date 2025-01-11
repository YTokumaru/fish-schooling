#include "coordinate.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

TEST(InnerTest, RadiusZero)
{
  double radius = 0.0;
  auto result = getInnerCells(radius);
  EXPECT_TRUE(result.empty());
}

TEST(InnerTest, RadiusLessThanRootThree)
{
  double radius = std::sqrt(2.9);
  auto result = getInnerCells(radius);
  EXPECT_TRUE(result.empty());
}

TEST(InnerTest, RadiusRootThree)
{
  double radius = std::sqrt(3);
  auto result = getInnerCells(radius);
  std::vector<std::array<int, 3>> expected = { { 0, 0, 0 } };
  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerTest, RadiusTwo)
{
  double radius = 2.0;
  auto result = getInnerCells(radius);
  std::vector<std::array<int, 3>> expected = { { 0, 0, 0 } };
  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerTest, RadiusRootSix)
{
  double radius = std::sqrt(6);
  auto result = getInnerCells(radius);
  std::vector<std::array<int, 3>> expected = {
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
  double radius1 = 0.0;
  double radius2 = 0.0;
  auto result = getInnerBetween(radius1, radius2);
  EXPECT_TRUE(result.empty());
}

TEST(InnerBetweenTest, OneRadiusZero)
{
  double radius1 = 0.0;
  double radius2 = 5.0;
  auto result = getInnerBetween(radius1, radius2);
  auto expected = getInnerCells(radius2);
  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(InnerBetweenTest, RadiusLessThanOrEqualToPointEight)
{
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

TEST(InnerBetweenTest, InnerIsEmpty)
{
  double radius1 = 0.5;
  double radius2 = 2.0;
  auto result = getInnerBetween(radius1, radius2);
  EXPECT_TRUE(result.empty());
}