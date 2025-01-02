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