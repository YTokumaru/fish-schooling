#include <gtest/gtest.h>

#include "coordinate.hpp"
#include "fish.hpp"
#include "simulation.hpp"

using namespace testing;

// NOLINTBEGIN(readability-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

TEST(FishTest, DefaultConstructor)
{
  const Fish fish;
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
  const Fish fish(
    { .x = 1.0, .y = 2.0, .z = 3.0 }, { .x = 0.1, .y = 0.2, .z = 0.3 }, { .x = 0.01, .y = 0.02, .z = 0.03 }, 0.5);
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
  const Fish fish({ .x = 0, .y = 0, .z = 0 }, { .x = 3, .y = 4, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 0);
  ASSERT_DOUBLE_EQ(fish.speed(), 5.0);
}

TEST(FishTest, Update)
{
  Fish fish({ .x = 0, .y = 0, .z = 0 }, { .x = 1, .y = 1, .z = 1 }, { .x = 0.1, .y = 0.1, .z = 0.1 }, 15.0);
  const SimParam sim_param{
    .length = 100, .n_fish = 1000, .max_steps = 1000, .delta_t = 0.01, .snapshot_interval = 100
  };
  const FishParam fish_param{ .vel_standard = 1.5,
    .vel_repulsion = 1.5,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 1.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 15.0,
    .attraction_duration = 1.0 };
  fish.update(sim_param, fish_param);
  ASSERT_NEAR(fish.getPosition().x, (1.0 + 0.1 * 0.01) * 0.01, 1e-6);
  ASSERT_NEAR(fish.getPosition().y, (1.0 + 0.1 * 0.01) * 0.01, 1e-6);
  ASSERT_NEAR(fish.getPosition().z, (1.0 + 0.1 * 0.01) * 0.01, 1e-6);
  ASSERT_DOUBLE_EQ(fish.getVelocity().x, 1.0 + (0.1 * 0.01));
  ASSERT_DOUBLE_EQ(fish.getVelocity().y, 1.0 + (0.1 * 0.01));
  ASSERT_DOUBLE_EQ(fish.getVelocity().z, 1.0 + (0.1 * 0.01));
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().x, 0.0);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().y, 0.0);
  ASSERT_DOUBLE_EQ(fish.getDeltaVelocity().z, 0.0);
  ASSERT_DOUBLE_EQ(fish.getLambda(), 15.0 - (15.0 / 100));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(readability-magic-numbers)
