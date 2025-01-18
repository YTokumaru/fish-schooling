#include "coordinate.hpp"
#include "eom.hpp"
#include "fish.hpp"
#include "simulation.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <vector>

using namespace testing;

TEST(EOMTest, GFactor)
{
  // Test the g factor for the repulsion interaction
  // The g factor is defined as the ratio of the body length to the distance between the fish
  // If the distance is less than the body length, the g factor is the body length divided by the distance
  // If the distance is more than the body length, the g factor is 1
  EXPECT_DOUBLE_EQ(g(0.5, 1.0), 2.0);
  EXPECT_DOUBLE_EQ(g(1.0, 1.0), 1.0);
  EXPECT_DOUBLE_EQ(g(1.5, 1.0), 1.0);

  EXPECT_DOUBLE_EQ(g(0.5, 0.5), 1.0);
}

TEST(EOMTest, SelfPropulsion)
{
  // NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  const Fish fish({ .x = 0, .y = 0, .z = 0 }, { .x = 0.5, .y = 0.6, .z = 0.7 }, { .x = 0, .y = 0, .z = 0 }, 0);
  const FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 1.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  auto delta_v = calcSelfPropulsion(fish, fish_param);

  EXPECT_DOUBLE_EQ(fish.speed(), sqrt((0.5 * 0.5) + (0.6 * 0.6) + (0.7 * 0.7)));
  EXPECT_DOUBLE_EQ(delta_v.x, (1 / sqrt((0.5 * 0.5) + (0.6 * 0.6) + (0.7 * 0.7)) - 1.0) * 0.5);
  EXPECT_DOUBLE_EQ(delta_v.y, (1 / sqrt((0.5 * 0.5) + (0.6 * 0.6) + (0.7 * 0.7)) - 1.0) * 0.6);
  EXPECT_DOUBLE_EQ(delta_v.z, (1 / sqrt((0.5 * 0.5) + (0.6 * 0.6) + (0.7 * 0.7)) - 1.0) * 0.7);

  EXPECT_FALSE(std::isnan(delta_v.x));
  EXPECT_FALSE(std::isnan(delta_v.y));
  EXPECT_FALSE(std::isnan(delta_v.z));
}

TEST(EOMTest, Repulsion)
{
  // Place two fish in the vicinity of each other and check the distance
  // Note that the distance between the two is less than the repulsion radius
  // is confined to the xy plane
  // NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish1({ .x = 0, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 0);
  // NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish2({ .x = 0.5, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 0);

  const SimParam sim_param{
    .length = 32,
    .n_fish = 2,
    .max_steps = 100,
    .delta_t = 0.1,
    .snapshot_interval = 10,
  };

  const FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 0.5,
    .attraction_radius = 7.5,
    .n_cog = 1,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
    std::vector<std::vector<std::vector<Fish *>>>(
      sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));
  cells[0][0][0].push_back(&fish1);
  cells[0][0][0].push_back(&fish2);

  auto inner = getInnerCells(fish_param.repulsion_radius);
  auto boundary = getBoundaryCells(fish_param.repulsion_radius);

  auto [delta_v_1, n_fish_1] = calcRepulsion(fish1, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_1, 1);
  fish1.setDeltaVelocity(delta_v_1);

  auto [delta_v_2, n_fish_2] = calcRepulsion(fish2, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_2, 1);
  fish2.setDeltaVelocity(delta_v_2);

  EXPECT_DOUBLE_EQ(delta_v_1.x, -2);
  EXPECT_DOUBLE_EQ(delta_v_1.y, -2);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);

  EXPECT_DOUBLE_EQ(delta_v_2.x, 2);
  EXPECT_DOUBLE_EQ(delta_v_2.y, -2);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  // The delta velocities should be equal and opposite
  EXPECT_DOUBLE_EQ(delta_v_1.x, -delta_v_2.x);
  EXPECT_TRUE(delta_v_1.x < 0);
  EXPECT_TRUE(delta_v_2.x > 0);
  EXPECT_DOUBLE_EQ(delta_v_1.y, delta_v_2.y);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  EXPECT_TRUE(std::isfinite(delta_v_1.x));
  EXPECT_TRUE(std::isfinite(delta_v_1.y));
  EXPECT_TRUE(std::isfinite(delta_v_1.z));
}

TEST(EOMTest, RepulsionOverBoundary)
{
  // Place two fish in the vicinity of each other and check the distance
  // Note that the distance between the two is less than the repulsion radius
  // Also note that the movement of the fish is confined to the xy plane
  // NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish1({ .x = 9.75, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 0);
  // NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish2({ .x = 0.25, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 0);

  const SimParam sim_param{ .length = 10, .n_fish = 2, .max_steps = 100, .delta_t = 0.1, .snapshot_interval = 10 };

  const FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 0.6,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
    std::vector<std::vector<std::vector<Fish *>>>(
      sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));
  cells[static_cast<unsigned long>(fish1.getPosition().x)][0][0].push_back(&fish1);
  cells[0][0][0].push_back(&fish2);

  auto inner = getInnerCells(fish_param.repulsion_radius);
  auto boundary = getBoundaryCells(fish_param.repulsion_radius);

  auto [delta_v_1, n_fish_1] = calcRepulsion(fish1, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_1, 1);
  fish1.setDeltaVelocity(delta_v_1);

  auto [delta_v_2, n_fish_2] = calcRepulsion(fish2, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_2, 1);
  fish2.setDeltaVelocity(delta_v_2);

  EXPECT_DOUBLE_EQ(delta_v_1.x, -2);
  EXPECT_DOUBLE_EQ(delta_v_1.y, -2);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);

  EXPECT_DOUBLE_EQ(delta_v_2.x, 2);
  EXPECT_DOUBLE_EQ(delta_v_2.y, -2);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  // The delta velocities should be equal and opposite
  EXPECT_DOUBLE_EQ(delta_v_1.x, -delta_v_2.x);
  EXPECT_TRUE(delta_v_1.x < 0);
  EXPECT_TRUE(delta_v_2.x > 0);
  EXPECT_DOUBLE_EQ(delta_v_1.y, delta_v_2.y);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  EXPECT_TRUE(std::isfinite(delta_v_1.x));
  EXPECT_TRUE(std::isfinite(delta_v_1.y));
  EXPECT_TRUE(std::isfinite(delta_v_1.z));
}

TEST(EOMTest, Attraction)
{
  // Place two fish in the vicinity of each other and check the distance
  // Note that the distance between the two is more than the repulsion radius and less than the attraction radius
  // Also note that the movement of the fish is confined to the xy plane
  // Make sure to set lambda to a positive value
  // NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish1({ .x = 0, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 3);
  // NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish2({ .x = 5.0, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 3);

  const SimParam sim_param{ .length = 32, .n_fish = 2, .max_steps = 100, .delta_t = 0.1, .snapshot_interval = 10 };

  const FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 4.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
    std::vector<std::vector<std::vector<Fish *>>>(
      sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));
  cells[0][0][0].push_back(&fish1);
  cells[static_cast<unsigned long>(fish2.getPosition().x)][0][0].push_back(&fish2);

  auto inner = getInnerBetween(fish_param.repulsion_radius, fish_param.attraction_radius);
  auto boundary = getBoundaryBetween(fish_param.repulsion_radius, fish_param.attraction_radius);

  auto [delta_v_1, n_fish_1] = calcAttraction(fish1, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_1, 1);

  auto [delta_v_2, n_fish_2] = calcAttraction(fish2, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_2, 1);

  EXPECT_DOUBLE_EQ(delta_v_1.x, 3 * 7.5);
  EXPECT_DOUBLE_EQ(delta_v_1.y, -3.0);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.x, -3 * 7.5);
  EXPECT_DOUBLE_EQ(delta_v_2.y, -3.0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  // The movement is confined and must be symmetrical in the xy plane
  EXPECT_DOUBLE_EQ(delta_v_1.x, -delta_v_2.x);
  EXPECT_TRUE(delta_v_1.x > 0);
  EXPECT_TRUE(delta_v_2.x < 0);
  EXPECT_DOUBLE_EQ(delta_v_1.y, delta_v_2.y);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  EXPECT_TRUE(std::isfinite(delta_v_1.x));
  EXPECT_TRUE(std::isfinite(delta_v_1.y));
  EXPECT_TRUE(std::isfinite(delta_v_1.z));
}

TEST(EOMTest, AttractionOverBoundary)
{
  // Place two fish in the vicinity of each other and check the distance
  // Note that the distance between the two is more than the repulsion radius and less than the attraction radius
  // Also note that the movement of the fish is confined to the xy plane
  // Make sure to set lambda to a positive value
  //// NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish1({ .x = 32 - 0.25, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 3);
  //// NOLINTNEXTLINE(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
  Fish fish2({ .x = 4.75, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 3);

  const SimParam sim_param{ .length = 32, .n_fish = 2, .max_steps = 100, .delta_t = 0.1, .snapshot_interval = 10 };

  const FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 4.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
    std::vector<std::vector<std::vector<Fish *>>>(
      sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));
  cells[static_cast<unsigned long>(fish1.getPosition().x)][0][0].push_back(&fish1);
  cells[static_cast<unsigned long>(fish2.getPosition().x)][0][0].push_back(&fish2);

  auto inner = getInnerBetween(fish_param.repulsion_radius, fish_param.attraction_radius);
  auto boundary = getBoundaryBetween(fish_param.repulsion_radius, fish_param.attraction_radius);

  auto [delta_v_1, n_fish_1] = calcAttraction(fish1, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_1, 1);

  auto [delta_v_2, n_fish_2] = calcAttraction(fish2, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_2, 1);

  EXPECT_DOUBLE_EQ(delta_v_1.x, 3 * 7.5);
  EXPECT_DOUBLE_EQ(delta_v_1.y, -3.0);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.x, -3 * 7.5);
  EXPECT_DOUBLE_EQ(delta_v_2.y, -3.0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  // The movement is confined and must be symmetrical in the xy plane
  EXPECT_DOUBLE_EQ(delta_v_1.x, -delta_v_2.x);
  EXPECT_TRUE(delta_v_1.x > 0);
  EXPECT_TRUE(delta_v_2.x < 0);
  EXPECT_DOUBLE_EQ(delta_v_1.y, delta_v_2.y);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  EXPECT_TRUE(std::isfinite(delta_v_1.x));
  EXPECT_TRUE(std::isfinite(delta_v_1.y));
  EXPECT_TRUE(std::isfinite(delta_v_1.z));
}

TEST(EOMTest, NoOtherFishAttraction)
{
  Fish fish({ .x = 0, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 1);

  const SimParam sim_param{ .length = 32, .n_fish = 1, .max_steps = 100, .delta_t = 0.1, .snapshot_interval = 10 };

  const FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 4.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
    std::vector<std::vector<std::vector<Fish *>>>(
      sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));
  cells[0][0][0].push_back(&fish);

  auto inner = getInnerBetween(fish_param.repulsion_radius, fish_param.attraction_radius);
  auto boundary = getBoundaryBetween(fish_param.repulsion_radius, fish_param.attraction_radius);

  auto [delta_v, n_fish] = calcAttraction(fish, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish, 0);
  EXPECT_DOUBLE_EQ(delta_v.x, 0);
  EXPECT_DOUBLE_EQ(delta_v.y, 0);
  EXPECT_DOUBLE_EQ(delta_v.z, 0);

  EXPECT_TRUE(std::isfinite(delta_v.x));
  EXPECT_TRUE(std::isfinite(delta_v.y));
  EXPECT_TRUE(std::isfinite(delta_v.z));
}

TEST(EOMTest, NoOtherFishRepulsion)
{
  Fish fish({ .x = 0, .y = 0, .z = 0 }, { .x = 0, .y = 1, .z = 0 }, { .x = 0, .y = 0, .z = 0 }, 1);

  const SimParam sim_param{ .length = 32, .n_fish = 1, .max_steps = 100, .delta_t = 0.1, .snapshot_interval = 10 };

  const FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 4.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
    std::vector<std::vector<std::vector<Fish *>>>(
      sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));
  cells[0][0][0].push_back(&fish);

  auto inner = getInnerCells(fish_param.repulsion_radius);
  auto boundary = getBoundaryCells(fish_param.repulsion_radius);

  auto [delta_v, n_fish] = calcRepulsion(fish, sim_param, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish, 0);
  EXPECT_DOUBLE_EQ(delta_v.x, 0);
  EXPECT_DOUBLE_EQ(delta_v.y, 0);
  EXPECT_DOUBLE_EQ(delta_v.z, 0);

  EXPECT_TRUE(std::isfinite(delta_v.x));
  EXPECT_TRUE(std::isfinite(delta_v.y));
  EXPECT_TRUE(std::isfinite(delta_v.z));
}
