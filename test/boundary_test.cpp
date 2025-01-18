#include "coordinate.hpp"
#include <array>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace testing;

TEST(BoundaryTest, RadiusZero)
{
  const double radius = 0.0;
  auto result = getBoundaryCells(radius);
  EXPECT_TRUE(result.empty());
}

TEST(BoundaryTest, RadiusEqualToPointFive)
{
  const double radius = 0.5;
  auto result = getBoundaryCells(radius);
  ASSERT_EQ(result.size(), 27);
  ASSERT_THAT(result,
    UnorderedElementsAreArray(std::vector<std::array<int, 3>>{ { 0, 0, 0 },
      { 0, 0, 1 },
      { 0, 1, 0 },
      { 1, 0, 0 },
      { 0, 0, -1 },
      { 0, -1, 0 },
      { -1, 0, 0 },
      { 1, 1, 0 },
      { 1, -1, 0 },
      { -1, 1, 0 },
      { -1, -1, 0 },
      { 1, 0, 1 },
      { -1, 0, 1 },
      { 0, 1, 1 },
      { 0, -1, 1 },
      { 1, 0, -1 },
      { -1, 0, -1 },
      { 0, 1, -1 },
      { 0, -1, -1 },
      { 1, 1, 1 },
      { 1, 1, -1 },
      { 1, -1, 1 },
      { 1, -1, -1 },
      { -1, 1, 1 },
      { -1, 1, -1 },
      { -1, -1, 1 },
      { -1, -1, -1 } }));
}

TEST(BoundaryTest, RadiusPointZeroSevenFive)
{
  const double radius = 0.75;
  auto result = getBoundaryCells(radius);
  ASSERT_EQ(result.size(), 27);
  const std::vector<std::array<int, 3>> expected = { { 0, 0, 0 },
    { 0, 0, 1 },
    { 0, 1, 0 },
    { 1, 0, 0 },
    { 0, 0, -1 },
    { 0, -1, 0 },
    { -1, 0, 0 },
    { 1, 1, 0 },
    { 1, -1, 0 },
    { -1, 1, 0 },
    { -1, -1, 0 },
    { 1, 0, 1 },
    { -1, 0, 1 },
    { 0, 1, 1 },
    { 0, -1, 1 },
    { 1, 0, -1 },
    { -1, 0, -1 },
    { 0, 1, -1 },
    { 0, -1, -1 },
    { 1, 1, 1 },
    { 1, 1, -1 },
    { 1, -1, 1 },
    { 1, -1, -1 },
    { -1, 1, 1 },
    { -1, 1, -1 },
    { -1, -1, 1 },
    { -1, -1, -1 } };

  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(BoundaryTest, RadiusTwo)
{
  const double radius = 2.0;
  auto result = getBoundaryCells(radius);
  const std::vector<std::array<int, 3>> expected = { { 0, 0, 1 },
    { 0, 1, 0 },
    { 1, 0, 0 },
    { 0, 0, -1 },
    { 0, -1, 0 },
    { -1, 0, 0 },
    { 1, 1, 0 },
    { 1, -1, 0 },
    { -1, 1, 0 },
    { -1, -1, 0 },
    { 1, 0, 1 },
    { -1, 0, 1 },
    { 0, 1, 1 },
    { 0, -1, 1 },
    { 1, 0, -1 },
    { -1, 0, -1 },
    { 0, 1, -1 },
    { 0, -1, -1 },
    { 1, 1, 1 },
    { 1, 1, -1 },
    { 1, -1, 1 },
    { 1, -1, -1 },
    { -1, 1, 1 },
    { -1, 1, -1 },
    { -1, -1, 1 },
    { -1, -1, -1 },
    { 0, 0, 2 },
    { 0, 0, -2 },
    { 0, 2, 0 },
    { 0, -2, 0 },
    { 2, 0, 0 },
    { -2, 0, 0 },
    { 2, 1, 0 },
    { -2, 1, 0 },
    { 2, -1, 0 },
    { -2, -1, 0 },
    { 2, 0, 1 },
    { -2, 0, 1 },
    { 2, 0, -1 },
    { -2, 0, -1 },
    { 0, 2, 1 },
    { 0, -2, 1 },
    { 0, 2, -1 },
    { 0, -2, -1 },
    { 1, 2, 0 },
    { 1, -2, 0 },
    { -1, 2, 0 },
    { -1, -2, 0 },
    { 1, 0, 2 },
    { 1, 0, -2 },
    { -1, 0, 2 },
    { -1, 0, -2 },
    { 0, 1, 2 },
    { 0, 1, -2 },
    { 0, -1, 2 },
    { 0, -1, -2 },
    { 2, 2, 0 },
    { 2, -2, 0 },
    { -2, 2, 0 },
    { -2, -2, 0 },
    { 2, 0, 2 },
    { 2, 0, -2 },
    { -2, 0, 2 },
    { -2, 0, -2 },
    { 0, 2, 2 },
    { 0, -2, 2 },
    { 0, 2, -2 },
    { 0, -2, -2 },
    { 2, 1, 1 },
    { 2, 1, -1 },
    { 2, -1, 1 },
    { 2, -1, -1 },
    { -2, 1, 1 },
    { -2, 1, -1 },
    { -2, -1, 1 },
    { -2, -1, -1 },
    { 1, 2, 1 },
    { 1, 2, -1 },
    { 1, -2, 1 },
    { 1, -2, -1 },
    { -1, 2, 1 },
    { -1, 2, -1 },
    { -1, -2, 1 },
    { -1, -2, -1 },
    { 1, 1, 2 },
    { 1, -1, 2 },
    { 1, 1, -2 },
    { 1, -1, -2 },
    { -1, 1, 2 },
    { -1, -1, 2 },
    { -1, 1, -2 },
    { -1, -1, -2 },
    { 2, 2, 1 },
    { 2, 2, -1 },
    { 2, -2, 1 },
    { 2, -2, -1 },
    { -2, 2, 1 },
    { -2, 2, -1 },
    { -2, -2, 1 },
    { -2, -2, -1 },
    { 2, 1, 2 },
    { 2, -1, 2 },
    { 2, 1, -2 },
    { 2, -1, -2 },
    { -2, 1, 2 },
    { -2, -1, 2 },
    { -2, 1, -2 },
    { -2, -1, -2 },
    { 1, 2, 2 },
    { 1, -2, 2 },
    { 1, 2, -2 },
    { 1, -2, -2 },
    { -1, 2, 2 },
    { -1, -2, 2 },
    { -1, 2, -2 },
    { -1, -2, -2 },
    { 2, 2, 2 },
    { 2, 2, -2 },
    { 2, -2, 2 },
    { 2, -2, -2 },
    { -2, 2, 2 },
    { -2, 2, -2 },
    { -2, -2, 2 },
    { -2, -2, -2 } };

  EXPECT_THAT(result, ::testing::UnorderedElementsAreArray(expected));
}

TEST(BoundaryBetweenTest, RadiusZero)
{
  const double radius1 = 0.0;
  const double radius2 = 0.0;
  auto result = getBoundaryBetween(radius1, radius2);
  EXPECT_TRUE(result.empty());
}

TEST(BoundaryBetweenTest, RadiusLessThanOrEqualToPointFive)
{
  const double radius1 = 0.5;
  const double radius2 = 0.5;
  auto result = getBoundaryBetween(radius1, radius2);
  const std::vector<std::array<int, 3>> expected = { { 0, 0, 0 },
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
    { -1, 0, 0 },
    { 0, -1, 0 },
    { 0, 0, -1 },
    { 1, 1, 0 },
    { 1, -1, 0 },
    { -1, 1, 0 },
    { -1, -1, 0 },
    { 1, 0, 1 },
    { -1, 0, 1 },
    { 0, 1, 1 },
    { 0, -1, 1 },
    { 1, 0, -1 },
    { -1, 0, -1 },
    { 0, 1, -1 },
    { 0, -1, -1 },
    { 1, 1, 1 },
    { 1, 1, -1 },
    { 1, -1, 1 },
    { 1, -1, -1 },
    { -1, 1, 1 },
    { -1, 1, -1 },
    { -1, -1, 1 },
    { -1, -1, -1 } };
  ASSERT_THAT(result, UnorderedElementsAreArray(expected));
}

TEST(BoundaryBetweenTest, SameRadius)
{
  const double radius = 2.5;
  auto result = getBoundaryBetween(radius, radius);

  auto other_result = getBoundaryCells(radius);

  EXPECT_THAT(result, UnorderedElementsAreArray(other_result));
}

TEST(BoundaryBetweenTest, RadiusFlipped)
{
  const double radius1 = 4.5;
  const double radius2 = 1.5;
  auto result1 = getBoundaryBetween(radius1, radius2);

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  auto result2 = getBoundaryBetween(radius2, radius1);

  EXPECT_THAT(result1, UnorderedElementsAreArray(result2));
}
