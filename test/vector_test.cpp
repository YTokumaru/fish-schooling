#include "coordinate.hpp"
#include <gtest/gtest.h>

TEST(Vect3Test, DefaultConstructor)
{
  Vect3 vec{};
  EXPECT_DOUBLE_EQ(vec.x, 0.0);
  EXPECT_DOUBLE_EQ(vec.y, 0.0);
  EXPECT_DOUBLE_EQ(vec.z, 0.0);
}

TEST(Vect3Test, ParameterizedConstructor)
{
  const Vect3 vec{ 1.0, 2.0, 3.0 };
  EXPECT_DOUBLE_EQ(vec.x, 1.0);
  EXPECT_DOUBLE_EQ(vec.y, 2.0);
  EXPECT_DOUBLE_EQ(vec.z, 3.0);
}

TEST(Vect3Test, Addition)
{
  const Vect3 vec1{ 1.0, 2.0, 3.0 };
  const Vect3 vec2{ 4.0, 5.0, 6.0 };
  Vect3 result = vec1 + vec2;
  EXPECT_DOUBLE_EQ(result.x, 5.0);
  EXPECT_DOUBLE_EQ(result.y, 7.0);
  EXPECT_DOUBLE_EQ(result.z, 9.0);
}

TEST(Vect3Test, CompoundAddition)
{
  Vect3 vec1{ 1.0, 2.0, 3.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const Vect3 vec2{ 4.0, 5.0, 6.0 };
  vec1 += vec2;
  EXPECT_DOUBLE_EQ(vec1.x, 5.0);
  EXPECT_DOUBLE_EQ(vec1.y, 7.0);
  EXPECT_DOUBLE_EQ(vec1.z, 9.0);
}

TEST(Vect3Test, Subtraction)
{
  const Vect3 vec1{ 4.0, 5.0, 6.0 };
  const Vect3 vec2{ 1.0, 2.0, 3.0 };
  Vect3 result = vec1 - vec2;
  EXPECT_DOUBLE_EQ(result.x, 3.0);
  EXPECT_DOUBLE_EQ(result.y, 3.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Vect3Test, CompoundSubtraction)
{
  Vect3 vec1{ 4.0, 5.0, 6.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const Vect3 vec2{ 1.0, 2.0, 3.0 };
  vec1 -= vec2;
  EXPECT_DOUBLE_EQ(vec1.x, 3.0);
  EXPECT_DOUBLE_EQ(vec1.y, 3.0);
  EXPECT_DOUBLE_EQ(vec1.z, 3.0);
}

TEST(Vect3Test, ScalarMultiplication)
{
  const Vect3 vec{ 1.0, 2.0, 3.0 };
  const double scalar = 2.0;
  Vect3 result1 = vec * scalar;
  Vect3 result2 = scalar * vec;
  EXPECT_DOUBLE_EQ(result1.x, 2.0);
  EXPECT_DOUBLE_EQ(result1.y, 4.0);
  EXPECT_DOUBLE_EQ(result1.z, 6.0);
  EXPECT_DOUBLE_EQ(result2.x, 2.0);
  EXPECT_DOUBLE_EQ(result2.y, 4.0);
  EXPECT_DOUBLE_EQ(result2.z, 6.0);
}

TEST(Vect3Test, CompoundScalarMultiplication)
{
  Vect3 vec{ 1.0, 2.0, 3.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const double scalar = 2.0;
  vec *= scalar;
  EXPECT_DOUBLE_EQ(vec.x, 2.0);
  EXPECT_DOUBLE_EQ(vec.y, 4.0);
  EXPECT_DOUBLE_EQ(vec.z, 6.0);
}

TEST(Vect3Test, ScalarDivision)
{
  const Vect3 vec{ 2.0, 4.0, 6.0 };
  const double scalar = 2.0;
  Vect3 result = vec / scalar;
  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 2.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Vect3Test, CompoundScalarDivision)
{
  Vect3 vec{ 2.0, 4.0, 6.0 };// NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const double scalar = 2.0;
  vec /= scalar;
  EXPECT_DOUBLE_EQ(vec.x, 1.0);
  EXPECT_DOUBLE_EQ(vec.y, 2.0);
  EXPECT_DOUBLE_EQ(vec.z, 3.0);
}

TEST(Vect3Test, vect12WithLengthSamePoint)
{
  Vect3 vec1{ 0.0, 0.0, 0.0 };
  Vect3 vec2{ 0.0, 0.0, 0.0 };
  const unsigned int length = 10;
  Vect3 result1 = vect12(vec1, vec2, length);
  EXPECT_DOUBLE_EQ(result1.x, 0.0);
  EXPECT_DOUBLE_EQ(result1.y, 0.0);
  EXPECT_DOUBLE_EQ(result1.z, 0.0);

  const Vect3 vec3{ 10.0, 0.0, 0.0 };
  Vect3 result2 = vect12(vec1, vec3, length);
  EXPECT_DOUBLE_EQ(result2.x, 0.0);
  EXPECT_DOUBLE_EQ(result2.y, 0.0);
  EXPECT_DOUBLE_EQ(result2.z, 0.0);

  const Vect3 vec4{ 0.0, 10.0, 0.0 };
  Vect3 result3 = vect12(vec1, vec4, length);
  EXPECT_DOUBLE_EQ(result3.x, 0.0);
  EXPECT_DOUBLE_EQ(result3.y, 0.0);
  EXPECT_DOUBLE_EQ(result3.z, 0.0);

  const Vect3 vec5{ 0.0, 0.0, 10.0 };
  Vect3 result4 = vect12(vec1, vec5, length);
  EXPECT_DOUBLE_EQ(result4.x, 0.0);
  EXPECT_DOUBLE_EQ(result4.y, 0.0);
  EXPECT_DOUBLE_EQ(result4.z, 0.0);

  const Vect3 vec6{ 10.0, 10.0, 0.0 };
  Vect3 result5 = vect12(vec1, vec6, length);
  EXPECT_DOUBLE_EQ(result5.x, 0.0);
  EXPECT_DOUBLE_EQ(result5.y, 0.0);
  EXPECT_DOUBLE_EQ(result5.z, 0.0);

  const Vect3 vec7{ 10.0, 0.0, 10.0 };
  Vect3 result6 = vect12(vec1, vec7, length);
  EXPECT_DOUBLE_EQ(result6.x, 0.0);
  EXPECT_DOUBLE_EQ(result6.y, 0.0);
  EXPECT_DOUBLE_EQ(result6.z, 0.0);

  const Vect3 vec8{ 0.0, 10.0, 10.0 };
  Vect3 result7 = vect12(vec1, vec8, length);
  EXPECT_DOUBLE_EQ(result7.x, 0.0);
  EXPECT_DOUBLE_EQ(result7.y, 0.0);
  EXPECT_DOUBLE_EQ(result7.z, 0.0);

  const Vect3 vec9{ 10.0, 10.0, 10.0 };
  Vect3 result8 = vect12(vec1, vec9, length);
  EXPECT_DOUBLE_EQ(result8.x, 0.0);
  EXPECT_DOUBLE_EQ(result8.y, 0.0);
  EXPECT_DOUBLE_EQ(result8.z, 0.0);
}

TEST(Vect3Test, vect12WithLength)
{
  const Vect3 vec1{ 1.0, 1.0, 0.0 };
  const Vect3 vec2{ 9.0, 9.0, 0.0 };
  const unsigned int length = 10;
  Vect3 result1 = vect12(vec1, vec2, length);
  EXPECT_DOUBLE_EQ(result1.x, -2.0);
  EXPECT_DOUBLE_EQ(result1.y, -2.0);
  EXPECT_DOUBLE_EQ(result1.z, 0.0);

  Vect3 result2 = vect12(vec2, vec1, length);
  EXPECT_DOUBLE_EQ(result2.x, 2.0);
  EXPECT_DOUBLE_EQ(result2.y, 2.0);
  EXPECT_DOUBLE_EQ(result2.z, 0.0);

  const Vect3 vec3{ 1.0, 0.0, 2.0 };
  const Vect3 vec4{ 9.0, 0.0, 8.0 };
  Vect3 result3 = vect12(vec3, vec4, length);
  EXPECT_DOUBLE_EQ(result3.x, -2.0);
  EXPECT_DOUBLE_EQ(result3.y, 0.0);
  EXPECT_DOUBLE_EQ(result3.z, -4.0);

  Vect3 result4 = vect12(vec4, vec3, length);
  EXPECT_DOUBLE_EQ(result4.x, 2.0);
  EXPECT_DOUBLE_EQ(result4.y, 0.0);
  EXPECT_DOUBLE_EQ(result4.z, 4.0);
}