#include "coordinate.hpp"
#include "eom.hpp"
#include "fish.hpp"
#include "simulation.hpp"
#include "gmock/gmock.h"
#include <cmath>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

TEST(EOMTest, Gfactor)
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
  Fish fish({ 0, 0, 0 }, { 0.5, 0.6, 0.7 }, { 0, 0, 0 }, 0);
  FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 1.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  auto delta_v = calcSelfPropulsion(fish, fish_param);

  EXPECT_DOUBLE_EQ(fish.speed(), sqrt(0.5 * 0.5 + 0.6 * 0.6 + 0.7 * 0.7));
  EXPECT_DOUBLE_EQ(delta_v.x, (1 / sqrt(0.5 * 0.5 + 0.6 * 0.6 + 0.7 * 0.7) - 1.0) * 0.5);
  EXPECT_DOUBLE_EQ(delta_v.y, (1 / sqrt(0.5 * 0.5 + 0.6 * 0.6 + 0.7 * 0.7) - 1.0) * 0.6);
  EXPECT_DOUBLE_EQ(delta_v.z, (1 / sqrt(0.5 * 0.5 + 0.6 * 0.6 + 0.7 * 0.7) - 1.0) * 0.7);
}

TEST(EOMTest, Repulsion)
{
  // Place two fish in the vicinity of each other and check the distance
  // Note that the distance between the two is less than the repulsion radius
  // Also note that the movement of the fish is confined to the xy plane
  Fish fish1({ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, 0);
  Fish fish2({ 0.5, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, 0);

  FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 0.5,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(
    32, std::vector<std::vector<std::vector<Fish *>>>(32, std::vector<std::vector<Fish *>>(32)));
  cells[0][0][0].push_back(&fish1);
  cells[0][0][0].push_back(&fish2);

  auto inner = getInnerCells(fish_param.repulsion_radius);
  auto boundary = getBoundaryCells(fish_param.repulsion_radius);

  EXPECT_EQ(inner.size(), 0);
  EXPECT_THAT(boundary, UnorderedElementsAreArray(std::vector<std::array<int, 3>>({ { 0, 0, 0 } })));

  auto [delta_v_1, n_fish_1] = calcRepulsion(fish1, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_1, 1);
  fish1.setDeltaVelocity(delta_v_1);

  auto [delta_v_2, n_fish_2] = calcRepulsion(fish2, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_2, 1);
  fish2.setDeltaVelocity(delta_v_2);

  EXPECT_DOUBLE_EQ(delta_v_1.x, -2);
  EXPECT_DOUBLE_EQ(delta_v_1.y, -2);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);

  EXPECT_DOUBLE_EQ(delta_v_2.x, 2);
  EXPECT_DOUBLE_EQ(delta_v_2.y, -2);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);

  // The delta velocities should be euqal and opposite
  EXPECT_DOUBLE_EQ(delta_v_1.x, -delta_v_2.x);
  EXPECT_TRUE(delta_v_1.x < 0);
  EXPECT_TRUE(delta_v_2.x > 0);
  EXPECT_DOUBLE_EQ(delta_v_1.y, delta_v_2.y);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);
}

TEST(EOMTest, Attraction)
{
  // Place two fish in the vicinity of each other and check the distance
  // Note that the distance between the two is more than the repulsion radius and less than the attraction radius
  // Also note that the movement of the fish is confined to the xy plane
  // Make sure to set lambda to a positive value
  Fish fish1({ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, 1);
  Fish fish2({ 5.0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, 1);

  FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 4.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(
    32, std::vector<std::vector<std::vector<Fish *>>>(32, std::vector<std::vector<Fish *>>(32)));
  cells[0][0][0].push_back(&fish1);
  cells[5][0][0].push_back(&fish2);

  auto inner = getInnerBetween(fish_param.repulsion_radius, fish_param.attraction_radius);
  auto boundary = getBoundaryBetween(fish_param.repulsion_radius, fish_param.attraction_radius);

  auto [delta_v_1, n_fish_1] = calcAttraction(fish1, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_1, 1);

  auto [delta_v_2, n_fish_2] = calcAttraction(fish2, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish_2, 1);

  // The movement is confined and must be symmetrical in the xy plane
  EXPECT_DOUBLE_EQ(delta_v_1.x, -delta_v_2.x);
  EXPECT_TRUE(delta_v_1.x > 0);
  EXPECT_TRUE(delta_v_2.x < 0);
  EXPECT_DOUBLE_EQ(delta_v_1.y, delta_v_2.y);
  EXPECT_DOUBLE_EQ(delta_v_1.z, 0);
  EXPECT_DOUBLE_EQ(delta_v_2.z, 0);
}

TEST(EOMTest, NoOtherFishAttraction)
{
  Fish fish({ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, 1);

  FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 4.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(
    32, std::vector<std::vector<std::vector<Fish *>>>(32, std::vector<std::vector<Fish *>>(32)));
  cells[0][0][0].push_back(&fish);

  auto inner = getInnerBetween(fish_param.repulsion_radius, fish_param.attraction_radius);
  auto boundary = getBoundaryBetween(fish_param.repulsion_radius, fish_param.attraction_radius);

  auto [delta_v, n_fish] = calcAttraction(fish, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish, 0);
  EXPECT_DOUBLE_EQ(delta_v.x, 0);
  EXPECT_DOUBLE_EQ(delta_v.y, 0);
  EXPECT_DOUBLE_EQ(delta_v.z, 0);
}

TEST(EOMTest, NoOtherFishRepultion)
{
  Fish fish({ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 }, 1);

  FishParam fish_param{ .vel_standard = 1.0,
    .vel_repulsion = 1.0,
    .vel_escape = 7.5,
    .body_length = 1.0,
    .repulsion_radius = 4.0,
    .attraction_radius = 7.5,
    .n_cog = 3,
    .attraction_str = 10.0,
    .attraction_duration = 0.1 };

  // Create dummy cells
  std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(
    32, std::vector<std::vector<std::vector<Fish *>>>(32, std::vector<std::vector<Fish *>>(32)));
  cells[0][0][0].push_back(&fish);

  auto inner = getInnerCells(fish_param.repulsion_radius);
  auto boundary = getBoundaryCells(fish_param.repulsion_radius);

  auto [delta_v, n_fish] = calcRepulsion(fish, fish_param, cells, boundary, inner);
  EXPECT_EQ(n_fish, 0);
  EXPECT_DOUBLE_EQ(delta_v.x, 0);
  EXPECT_DOUBLE_EQ(delta_v.y, 0);
  EXPECT_DOUBLE_EQ(delta_v.z, 0);
}