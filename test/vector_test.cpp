#include "coordinate.hpp"
#include <cmath>
#include <gtest/gtest.h>

TEST(Vect3Test, DefaultConstructor)
{
  const Vect3 vec{};
  EXPECT_DOUBLE_EQ(vec.x, 0.0);
  EXPECT_DOUBLE_EQ(vec.y, 0.0);
  EXPECT_DOUBLE_EQ(vec.z, 0.0);
}

TEST(Vect3Test, ParameterizedConstructor)
{
  const Vect3 vec{ .x = 1.0, .y = 2.0, .z = 3.0 };
  EXPECT_DOUBLE_EQ(vec.x, 1.0);
  EXPECT_DOUBLE_EQ(vec.y, 2.0);
  EXPECT_DOUBLE_EQ(vec.z, 3.0);
}

TEST(Vect3Test, Addition)
{
  const Vect3 vec1{ .x = 1.0, .y = 2.0, .z = 3.0 };
  const Vect3 vec2{ .x = 4.0, .y = 5.0, .z = 6.0 };
  const Vect3 result = vec1 + vec2;
  EXPECT_DOUBLE_EQ(result.x, 5.0);
  EXPECT_DOUBLE_EQ(result.y, 7.0);
  EXPECT_DOUBLE_EQ(result.z, 9.0);
}

TEST(Vect3Test, CompoundAddition)
{
  Vect3 vec1{ .x = 1.0, .y = 2.0, .z = 3.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const Vect3 vec2{ .x = 4.0, .y = 5.0, .z = 6.0 };
  vec1 += vec2;
  EXPECT_DOUBLE_EQ(vec1.x, 5.0);
  EXPECT_DOUBLE_EQ(vec1.y, 7.0);
  EXPECT_DOUBLE_EQ(vec1.z, 9.0);
}

TEST(Vect3Test, Subtraction)
{
  const Vect3 vec1{ .x = 4.0, .y = 5.0, .z = 6.0 };
  const Vect3 vec2{ .x = 1.0, .y = 2.0, .z = 3.0 };
  const Vect3 result = vec1 - vec2;
  EXPECT_DOUBLE_EQ(result.x, 3.0);
  EXPECT_DOUBLE_EQ(result.y, 3.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Vect3Test, CompoundSubtraction)
{
  Vect3 vec1{ .x = 4.0, .y = 5.0, .z = 6.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const Vect3 vec2{ .x = 1.0, .y = 2.0, .z = 3.0 };
  vec1 -= vec2;
  EXPECT_DOUBLE_EQ(vec1.x, 3.0);
  EXPECT_DOUBLE_EQ(vec1.y, 3.0);
  EXPECT_DOUBLE_EQ(vec1.z, 3.0);
}

TEST(Vect3Test, ScalarMultiplication)
{
  const Vect3 vec{ .x = 1.0, .y = 2.0, .z = 3.0 };
  const double scalar = 2.0;
  const Vect3 result1 = vec * scalar;
  const Vect3 result2 = scalar * vec;
  EXPECT_DOUBLE_EQ(result1.x, 2.0);
  EXPECT_DOUBLE_EQ(result1.y, 4.0);
  EXPECT_DOUBLE_EQ(result1.z, 6.0);
  EXPECT_DOUBLE_EQ(result2.x, 2.0);
  EXPECT_DOUBLE_EQ(result2.y, 4.0);
  EXPECT_DOUBLE_EQ(result2.z, 6.0);
}

TEST(Vect3Test, CompoundScalarMultiplication)
{
  Vect3 vec{ .x = 1.0, .y = 2.0, .z = 3.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const double scalar = 2.0;
  vec *= scalar;
  EXPECT_DOUBLE_EQ(vec.x, 2.0);
  EXPECT_DOUBLE_EQ(vec.y, 4.0);
  EXPECT_DOUBLE_EQ(vec.z, 6.0);
}

TEST(Vect3Test, ScalarDivision)
{
  const Vect3 vec{ .x = 2.0, .y = 4.0, .z = 6.0 };
  const double scalar = 2.0;
  const Vect3 result = vec / scalar;
  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 2.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Vect3Test, CompoundScalarDivision)
{
  Vect3 vec{ .x = 2.0, .y = 4.0, .z = 6.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const double scalar = 2.0;
  vec /= scalar;
  EXPECT_DOUBLE_EQ(vec.x, 1.0);
  EXPECT_DOUBLE_EQ(vec.y, 2.0);
  EXPECT_DOUBLE_EQ(vec.z, 3.0);
}

TEST(Vect3Test, vect12WithLengthSamePoint)
{
  const Vect3 vec1{ .x = 0.0, .y = 0.0, .z = 0.0 };
  const Vect3 vec2{ .x = 0.0, .y = 0.0, .z = 0.0 };
  const unsigned int length = 10;
  const Vect3 result1 = vect12(vec1, vec2, length);
  EXPECT_DOUBLE_EQ(result1.x, 0.0);
  EXPECT_DOUBLE_EQ(result1.y, 0.0);
  EXPECT_DOUBLE_EQ(result1.z, 0.0);

  const Vect3 vec3{ .x = 10.0, .y = 0.0, .z = 0.0 };
  const Vect3 result2 = vect12(vec1, vec3, length);
  EXPECT_DOUBLE_EQ(result2.x, 0.0);
  EXPECT_DOUBLE_EQ(result2.y, 0.0);
  EXPECT_DOUBLE_EQ(result2.z, 0.0);

  const Vect3 vec4{ .x = 0.0, .y = 10.0, .z = 0.0 };
  const Vect3 result3 = vect12(vec1, vec4, length);
  EXPECT_DOUBLE_EQ(result3.x, 0.0);
  EXPECT_DOUBLE_EQ(result3.y, 0.0);
  EXPECT_DOUBLE_EQ(result3.z, 0.0);

  const Vect3 vec5{ .x = 0.0, .y = 0.0, .z = 10.0 };
  const Vect3 result4 = vect12(vec1, vec5, length);
  EXPECT_DOUBLE_EQ(result4.x, 0.0);
  EXPECT_DOUBLE_EQ(result4.y, 0.0);
  EXPECT_DOUBLE_EQ(result4.z, 0.0);

  const Vect3 vec6{ .x = 10.0, .y = 10.0, .z = 0.0 };
  const Vect3 result5 = vect12(vec1, vec6, length);
  EXPECT_DOUBLE_EQ(result5.x, 0.0);
  EXPECT_DOUBLE_EQ(result5.y, 0.0);
  EXPECT_DOUBLE_EQ(result5.z, 0.0);

  const Vect3 vec7{ .x = 10.0, .y = 0.0, .z = 10.0 };
  const Vect3 result6 = vect12(vec1, vec7, length);
  EXPECT_DOUBLE_EQ(result6.x, 0.0);
  EXPECT_DOUBLE_EQ(result6.y, 0.0);
  EXPECT_DOUBLE_EQ(result6.z, 0.0);

  const Vect3 vec8{ .x = 0.0, .y = 10.0, .z = 10.0 };
  const Vect3 result7 = vect12(vec1, vec8, length);
  EXPECT_DOUBLE_EQ(result7.x, 0.0);
  EXPECT_DOUBLE_EQ(result7.y, 0.0);
  EXPECT_DOUBLE_EQ(result7.z, 0.0);

  const Vect3 vec9{ .x = 10.0, .y = 10.0, .z = 10.0 };
  const Vect3 result8 = vect12(vec1, vec9, length);
  EXPECT_DOUBLE_EQ(result8.x, 0.0);
  EXPECT_DOUBLE_EQ(result8.y, 0.0);
  EXPECT_DOUBLE_EQ(result8.z, 0.0);
}

TEST(Vect3Test, vect12WithLength)
{
  const Vect3 vec1{ .x = 1.0, .y = 1.0, .z = 0.0 };
  const Vect3 vec2{ .x = 9.0, .y = 9.0, .z = 0.0 };
  const unsigned int length = 10;
  const Vect3 result1 = vect12(vec1, vec2, length);
  EXPECT_DOUBLE_EQ(result1.x, -2.0);
  EXPECT_DOUBLE_EQ(result1.y, -2.0);
  EXPECT_DOUBLE_EQ(result1.z, 0.0);

  const Vect3 result2 = vect12(vec2, vec1, length);
  EXPECT_DOUBLE_EQ(result2.x, 2.0);
  EXPECT_DOUBLE_EQ(result2.y, 2.0);
  EXPECT_DOUBLE_EQ(result2.z, 0.0);

  const Vect3 vec3{ .x = 1.0, .y = 0.0, .z = 2.0 };
  const Vect3 vec4{ .x = 9.0, .y = 0.0, .z = 8.0 };
  const Vect3 result3 = vect12(vec3, vec4, length);
  EXPECT_DOUBLE_EQ(result3.x, -2.0);
  EXPECT_DOUBLE_EQ(result3.y, 0.0);
  EXPECT_DOUBLE_EQ(result3.z, -4.0);

  const Vect3 result4 = vect12(vec4, vec3, length);
  EXPECT_DOUBLE_EQ(result4.x, 2.0);
  EXPECT_DOUBLE_EQ(result4.y, 0.0);
  EXPECT_DOUBLE_EQ(result4.z, 4.0);
}

TEST(Vect3Test, Periodic)
{
  const Vect3 vect1{ .x = 1.0, .y = 1.0, .z = 1.0 };
  const auto result1 = periodic(vect1, 2.0);

  EXPECT_DOUBLE_EQ(result1.x, 1.0);
  EXPECT_DOUBLE_EQ(result1.y, 1.0);
  EXPECT_DOUBLE_EQ(result1.z, 1.0);

  const Vect3 vect2{ .x = 3.0, .y = 3.0, .z = 3.0 };
  const auto result2 = periodic(vect2, 2.0);
  EXPECT_DOUBLE_EQ(result2.x, 1.0);
  EXPECT_DOUBLE_EQ(result2.y, 1.0);
  EXPECT_DOUBLE_EQ(result2.z, 1.0);

  const Vect3 vect3{ .x = -1.0, .y = -1.0, .z = -1.0 };
  const auto result3 = periodic(vect3, 2.0);
  EXPECT_DOUBLE_EQ(result3.x, 1.0);
  EXPECT_DOUBLE_EQ(result3.y, 1.0);
  EXPECT_DOUBLE_EQ(result3.z, 1.0);
}

TEST(Vect3Test, Normalize)
{
  const Vect3 vect1{ .x = 1.0, .y = 1.0, .z = 1.0 };
  const auto result1 = normalize(vect1);
  EXPECT_DOUBLE_EQ(result1.x, 1.0 / std::sqrt(3.0));
  EXPECT_DOUBLE_EQ(result1.y, 1.0 / std::sqrt(3.0));
  EXPECT_DOUBLE_EQ(result1.z, 1.0 / std::sqrt(3.0));

  const Vect3 vect2{ .x = 3.0, .y = 4.0, .z = 5.0 };
  const auto result2 = normalize(vect2);
  EXPECT_DOUBLE_EQ(result2.x, 3.0 / std::sqrt(50.0));
  EXPECT_DOUBLE_EQ(result2.y, 4.0 / std::sqrt(50.0));
  EXPECT_DOUBLE_EQ(result2.z, 5.0 / std::sqrt(50.0));

  const Vect3 vect3{ .x = 0.0, .y = 0.0, .z = 0.0 };
  const auto result3 = normalize(vect3);
  EXPECT_DOUBLE_EQ(result3.x, 0.0);
  EXPECT_DOUBLE_EQ(result3.y, 0.0);
  EXPECT_DOUBLE_EQ(result3.z, 0.0);

  const Vect3 vect4{ .x = 0.0, .y = 0.0, .z = 1.0 };
  const auto result4 = normalize(vect4);
  EXPECT_DOUBLE_EQ(result4.x, 0.0);
  EXPECT_DOUBLE_EQ(result4.y, 0.0);
  EXPECT_DOUBLE_EQ(result4.z, 1.0);

  const Vect3 vect5{ .x = 0.0, .y = 0.0, .z = -1.0 };
  const auto result5 = normalize(vect5);
  EXPECT_DOUBLE_EQ(result5.x, 0.0);
  EXPECT_DOUBLE_EQ(result5.y, 0.0);
  EXPECT_DOUBLE_EQ(result5.z, -1.0);

  const Vect3 vect6{ .x = 0.0, .y = 1.0, .z = 0.0 };
  const auto result6 = normalize(vect6);
  EXPECT_DOUBLE_EQ(result6.x, 0.0);
  EXPECT_DOUBLE_EQ(result6.y, 1.0);
  EXPECT_DOUBLE_EQ(result6.z, 0.0);

  const Vect3 vect7{ .x = 0.0, .y = -1.0, .z = 0.0 };
  const auto result7 = normalize(vect7);
  EXPECT_DOUBLE_EQ(result7.x, 0.0);
  EXPECT_DOUBLE_EQ(result7.y, -1.0);
  EXPECT_DOUBLE_EQ(result7.z, 0.0);

  const Vect3 vect8{ .x = 1.0, .y = 0.0, .z = 0.0 };
  const auto result8 = normalize(vect8);
  EXPECT_DOUBLE_EQ(result8.x, 1.0);
  EXPECT_DOUBLE_EQ(result8.y, 0.0);
  EXPECT_DOUBLE_EQ(result8.z, 0.0);

  const Vect3 vect9{ .x = -1.0, .y = 0.0, .z = 0.0 };
  const auto result9 = normalize(vect9);
  EXPECT_DOUBLE_EQ(result9.x, -1.0);
  EXPECT_DOUBLE_EQ(result9.y, 0.0);
  EXPECT_DOUBLE_EQ(result9.z, 0.0);
}