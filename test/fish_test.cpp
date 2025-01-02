#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fish.hpp"

using namespace testing;

// NOLINTBEGIN(readability-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

TEST(FishTest, DefaultConstructor)
{
  Fish fish;
  ASSERT_DOUBLE_EQ(fish.getPosition().x, 0.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().y, 0.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().z, 0.0);
  ASSERT_DOUBLE_EQ(fish.getVelocity().x, 0.0);
  ASSERT_DOUBLE_EQ(fish.getVelocity().y, 0.0);
  ASSERT_DOUBLE_EQ(fish.getVelocity().z, 0.0);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().x, 0.0);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().y, 0.0);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().z, 0.0);
  ASSERT_DOUBLE_EQ(fish.getLambda(), 0.0);
}

TEST(FishTest, ParameterizedConstructor)
{
  Fish fish({ 1.0, 2.0, 3.0 }, { 0.1, 0.2, 0.3 }, { 0.01, 0.02, 0.03 }, 0.5);
  ASSERT_DOUBLE_EQ(fish.getPosition().x, 1.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().y, 2.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().z, 3.0);
  ASSERT_DOUBLE_EQ(fish.getVelocity().x, 0.1);
  ASSERT_DOUBLE_EQ(fish.getVelocity().y, 0.2);
  ASSERT_DOUBLE_EQ(fish.getVelocity().z, 0.3);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().x, 0.01);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().y, 0.02);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().z, 0.03);
  ASSERT_DOUBLE_EQ(fish.getLambda(), 0.5);
}

TEST(FishTest, SettersAndGetters)
{
  Fish fish;
  fish.setPosition(4.0, 5.0, 6.0);
  fish.setVelocity(0.4, 0.5, 0.6);
  fish.setDeltaVelocity(0.04, 0.05, 0.06);
  fish.setLambda(0.8);

  ASSERT_DOUBLE_EQ(fish.getPosition().x, 4.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().y, 5.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().z, 6.0);
  ASSERT_DOUBLE_EQ(fish.getVelocity().x, 0.4);
  ASSERT_DOUBLE_EQ(fish.getVelocity().y, 0.5);
  ASSERT_DOUBLE_EQ(fish.getVelocity().z, 0.6);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().x, 0.04);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().y, 0.05);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().z, 0.06);
  ASSERT_DOUBLE_EQ(fish.getLambda(), 0.8);
}

TEST(FishTest, Speed)
{
  Fish fish({ 0, 0, 0 }, { 3, 4, 0 }, { 0, 0, 0 }, 0);
  ASSERT_DOUBLE_EQ(fish.speed(), 5.0);
}

TEST(FishTest, UpdatePosition)
{
  Fish fish({ 0, 0, 0 }, { 1, 1, 1 }, { 0, 0, 0 }, 0);
  fish.update(1.0, 100, 0.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().x, 1.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().y, 1.0);
  ASSERT_DOUBLE_EQ(fish.getPosition().z, 1.0);
}

TEST(FishTest, UpdateWithParams)
{
  Fish fish({ 0, 0, 0 }, { 1, 1, 1 }, { 0.1, 0.1, 0.1 }, 0.5);
  SimParam sim_param{ 100, 1000, 1000, 0.01 };
  FishParam fish_param{ 1.5, 1.5, 7.5, 1.0, 1.0, 7.5, 3, 15.0, 0.1 };
  fish.update(sim_param, fish_param);
  ASSERT_DOUBLE_EQ(fish.getVelocity().x, 1.1);
  ASSERT_DOUBLE_EQ(fish.getVelocity().y, 1.1);
  ASSERT_DOUBLE_EQ(fish.getVelocity().z, 1.1);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(readability-magic-numbers)