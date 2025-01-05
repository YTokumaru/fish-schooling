#include "coordinate.hpp"
#include <gtest/gtest.h>

TEST(Vect3Test, DefaultConstructor)
{
  Vect3 v{};
  EXPECT_DOUBLE_EQ(v.x, 0.0);
  EXPECT_DOUBLE_EQ(v.y, 0.0);
  EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Vect3Test, ParameterizedConstructor)
{
  Vect3 v{ 1.0, 2.0, 3.0 };
  EXPECT_DOUBLE_EQ(v.x, 1.0);
  EXPECT_DOUBLE_EQ(v.y, 2.0);
  EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vect3Test, Addition)
{
  Vect3 v1{ 1.0, 2.0, 3.0 };
  Vect3 v2{ 4.0, 5.0, 6.0 };
  Vect3 result = v1 + v2;
  EXPECT_DOUBLE_EQ(result.x, 5.0);
  EXPECT_DOUBLE_EQ(result.y, 7.0);
  EXPECT_DOUBLE_EQ(result.z, 9.0);
}

TEST(Vect3Test, CompoundAddition)
{
  Vect3 v1{ 1.0, 2.0, 3.0 };
  Vect3 v2{ 4.0, 5.0, 6.0 };
  v1 += v2;
  EXPECT_DOUBLE_EQ(v1.x, 5.0);
  EXPECT_DOUBLE_EQ(v1.y, 7.0);
  EXPECT_DOUBLE_EQ(v1.z, 9.0);
}

TEST(Vect3Test, Subtraction)
{
  Vect3 v1{ 4.0, 5.0, 6.0 };
  Vect3 v2{ 1.0, 2.0, 3.0 };
  Vect3 result = v1 - v2;
  EXPECT_DOUBLE_EQ(result.x, 3.0);
  EXPECT_DOUBLE_EQ(result.y, 3.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Vect3Test, CompoundSubtraction)
{
  Vect3 v1{ 4.0, 5.0, 6.0 };
  Vect3 v2{ 1.0, 2.0, 3.0 };
  v1 -= v2;
  EXPECT_DOUBLE_EQ(v1.x, 3.0);
  EXPECT_DOUBLE_EQ(v1.y, 3.0);
  EXPECT_DOUBLE_EQ(v1.z, 3.0);
}

TEST(Vect3Test, ScalarMultiplication)
{
  Vect3 v{ 1.0, 2.0, 3.0 };
  double scalar = 2.0;
  Vect3 result1 = v * scalar;
  Vect3 result2 = scalar * v;
  EXPECT_DOUBLE_EQ(result1.x, 2.0);
  EXPECT_DOUBLE_EQ(result1.y, 4.0);
  EXPECT_DOUBLE_EQ(result1.z, 6.0);
  EXPECT_DOUBLE_EQ(result2.x, 2.0);
  EXPECT_DOUBLE_EQ(result2.y, 4.0);
  EXPECT_DOUBLE_EQ(result2.z, 6.0);
}

TEST(Vect3Test, CompoundScalarMultiplication)
{
  Vect3 v{ 1.0, 2.0, 3.0 };
  double scalar = 2.0;
  v *= scalar;
  EXPECT_DOUBLE_EQ(v.x, 2.0);
  EXPECT_DOUBLE_EQ(v.y, 4.0);
  EXPECT_DOUBLE_EQ(v.z, 6.0);
}

TEST(Vect3Test, ScalarDivision)
{
  Vect3 v{ 2.0, 4.0, 6.0 };
  double scalar = 2.0;
  Vect3 result = v / scalar;
  EXPECT_DOUBLE_EQ(result.x, 1.0);
  EXPECT_DOUBLE_EQ(result.y, 2.0);
  EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Vect3Test, CompoundScalarDivision)
{
  Vect3 v{ 2.0, 4.0, 6.0 };
  double scalar = 2.0;
  v /= scalar;
  EXPECT_DOUBLE_EQ(v.x, 1.0);
  EXPECT_DOUBLE_EQ(v.y, 2.0);
  EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vect3Test, vect12WithLengthSamePoint)
{
  Vect3 v1{ 0.0, 0.0, 0.0 };
  Vect3 v2{ 0.0, 0.0, 0.0 };
  Vect3 result1 = vect12(v1, v2, 10);
  EXPECT_DOUBLE_EQ(result1.x, 0.0);
  EXPECT_DOUBLE_EQ(result1.y, 0.0);
  EXPECT_DOUBLE_EQ(result1.z, 0.0);

  Vect3 v3{ 10.0, 0.0, 0.0 };
  Vect3 result2 = vect12(v1, v3, 10);
  EXPECT_DOUBLE_EQ(result2.x, 0.0);
  EXPECT_DOUBLE_EQ(result2.y, 0.0);
  EXPECT_DOUBLE_EQ(result2.z, 0.0);

  Vect3 v4{ 0.0, 10.0, 0.0 };
  Vect3 result3 = vect12(v1, v4, 10);
  EXPECT_DOUBLE_EQ(result3.x, 0.0);
  EXPECT_DOUBLE_EQ(result3.y, 0.0);
  EXPECT_DOUBLE_EQ(result3.z, 0.0);

  Vect3 v5{ 0.0, 0.0, 10.0 };
  Vect3 result4 = vect12(v1, v5, 10);
  EXPECT_DOUBLE_EQ(result4.x, 0.0);
  EXPECT_DOUBLE_EQ(result4.y, 0.0);
  EXPECT_DOUBLE_EQ(result4.z, 0.0);

  Vect3 v6{ 10.0, 10.0, 0.0 };
  Vect3 result5 = vect12(v1, v6, 10);
  EXPECT_DOUBLE_EQ(result5.x, 0.0);
  EXPECT_DOUBLE_EQ(result5.y, 0.0);
  EXPECT_DOUBLE_EQ(result5.z, 0.0);

  Vect3 v7{ 10.0, 0.0, 10.0 };
  Vect3 result6 = vect12(v1, v7, 10);
  EXPECT_DOUBLE_EQ(result6.x, 0.0);
  EXPECT_DOUBLE_EQ(result6.y, 0.0);
  EXPECT_DOUBLE_EQ(result6.z, 0.0);

  Vect3 v8{ 0.0, 10.0, 10.0 };
  Vect3 result7 = vect12(v1, v8, 10);
  EXPECT_DOUBLE_EQ(result7.x, 0.0);
  EXPECT_DOUBLE_EQ(result7.y, 0.0);
  EXPECT_DOUBLE_EQ(result7.z, 0.0);

  Vect3 v9{ 10.0, 10.0, 10.0 };
  Vect3 result8 = vect12(v1, v9, 10);
  EXPECT_DOUBLE_EQ(result8.x, 0.0);
  EXPECT_DOUBLE_EQ(result8.y, 0.0);
  EXPECT_DOUBLE_EQ(result8.z, 0.0);
}

TEST(Vect3Test, vect12WithLength)
{
  Vect3 v1{ 1.0, 1.0, 0.0 };
  Vect3 v2{ 9.0, 9.0, 0.0 };
  Vect3 result1 = vect12(v1, v2, 10);
  EXPECT_DOUBLE_EQ(result1.x, -2.0);
  EXPECT_DOUBLE_EQ(result1.y, -2.0);
  EXPECT_DOUBLE_EQ(result1.z, 0.0);

  Vect3 result2 = vect12(v2, v1, 10);
  EXPECT_DOUBLE_EQ(result2.x, 2.0);
  EXPECT_DOUBLE_EQ(result2.y, 2.0);
  EXPECT_DOUBLE_EQ(result2.z, 0.0);

  Vect3 v3{ 1.0, 0.0, 2.0 };
  Vect3 v4{ 9.0, 0.0, 8.0 };
  Vect3 result3 = vect12(v3, v4, 10);
  EXPECT_DOUBLE_EQ(result3.x, -2.0);
  EXPECT_DOUBLE_EQ(result3.y, 0.0);
  EXPECT_DOUBLE_EQ(result3.z, -4.0);

  Vect3 result4 = vect12(v4, v3, 10);
  EXPECT_DOUBLE_EQ(result4.x, 2.0);
  EXPECT_DOUBLE_EQ(result4.y, 0.0);
  EXPECT_DOUBLE_EQ(result4.z, 4.0);
}