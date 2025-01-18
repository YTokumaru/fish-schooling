#include "io.hpp"

#include "simulation.hpp"
#include <cstdlib>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

using namespace testing;

// Mocking the YAML::LoadFile function
class MockYAML
{
public:
  MOCK_METHOD(YAML::Node, LoadFile, (const std::string &filename), ());
};

// Fixture for testing
class ConfigLoaderTest : public ::testing::Test
{
protected:
  [[maybe_unused]] SimParam sim_param{};
  [[maybe_unused]] FishParam fish_param{};

  YAML::Node validConfig;

  void SetUp() override
  {
    // Create a valid YAML::Node structure
    validConfig = YAML::Load(R"(
            simulation-params:
              length: 64
              n-fish: 1000
              max-steps: 1000
              delta-t: 0.01
              snapshot-interval: 100
            fish-params:
              vel-standard: 1.5
              vel-repulsion: 1.5
              vel-escape: 7.5
              body-length: 1.0
              repulsion-radius: 1.0
              attraction-radius: 7.5
              n-cog: 3
              attraction-strength: 15.0
              attraction-duration: 0.1
        )");
    // Explicitly reference the members to suppress unused warnings
    (void)sim_param;
    (void)fish_param;
  }
};

TEST_F(ConfigLoaderTest, ParseValidConfig)
{
  // Call the operator >> and check if parsing is correct
  ASSERT_EQ(validConfig >> sim_param, EXIT_SUCCESS);
  ASSERT_EQ(validConfig >> fish_param, EXIT_SUCCESS);

  // Validate sim_param
  EXPECT_EQ(sim_param.length, 64);
  EXPECT_EQ(sim_param.n_fish, 1000);
  EXPECT_EQ(sim_param.max_steps, 1000);
  EXPECT_DOUBLE_EQ(sim_param.delta_t, 0.01);

  // Validate fish_param
  EXPECT_DOUBLE_EQ(fish_param.vel_standard, 1.5);
  EXPECT_DOUBLE_EQ(fish_param.vel_repulsion, 1.5);
  EXPECT_DOUBLE_EQ(fish_param.vel_escape, 7.5);
  EXPECT_DOUBLE_EQ(fish_param.body_length, 1.0);
  EXPECT_DOUBLE_EQ(fish_param.repulsion_radius, 1.0);
  EXPECT_DOUBLE_EQ(fish_param.attraction_radius, 7.5);
  EXPECT_EQ(fish_param.n_cog, 3);
  EXPECT_DOUBLE_EQ(fish_param.attraction_str, 15.0);
  EXPECT_DOUBLE_EQ(fish_param.attraction_duration, 0.1);
}

TEST_F(ConfigLoaderTest, MissingSimParamsKey)
{
  const YAML::Node incompleteConfig = YAML::Load(R"(
        fish-params:
          vel-standard: 1.5
          vel-repulsion: 1.5
    )");

  EXPECT_EQ(incompleteConfig >> sim_param, EXIT_FAILURE);
}

TEST_F(ConfigLoaderTest, MissingFishParamsKey)
{
  const YAML::Node incompleteConfig = YAML::Load(R"(
        simulation-params:
          length: 64
          n-fish: 1000
    )");

  EXPECT_EQ(incompleteConfig >> fish_param, EXIT_FAILURE);
}

TEST_F(ConfigLoaderTest, WrongType)
{
  const YAML::Node wrongTypeConfig = YAML::Load(R"(
        simulation-params: 1000
        fish-params: 1000
    )");

  EXPECT_EQ(wrongTypeConfig >> sim_param, EXIT_FAILURE);
  EXPECT_EQ(wrongTypeConfig >> fish_param, EXIT_FAILURE);
}

TEST_F(ConfigLoaderTest, WrongDataType)
{
  const YAML::Node wrongDataTypeConfig = YAML::Load(R"(
        simulation-params:
          length: 
          n-fish: "1000"
          max-steps: "1000"
          delta-t: "0.01"
          snapshot-interval: "100"
        fish-params:
          vel-standard: "1.5"
          vel-repulsion: "1.5"
          vel-escape: "7.5"
          body-length: "1.0"
          repulsion-radius: "1.0"
          attraction-radius: "7.5"
          n-cog: 3.6
          attraction-strength: "15.0"
          attraction-duration: "0.1"
    )");

  EXPECT_EQ(wrongDataTypeConfig >> sim_param, EXIT_FAILURE);
  EXPECT_EQ(wrongDataTypeConfig >> fish_param, EXIT_FAILURE);
}

TEST_F(ConfigLoaderTest, NegativeValues)
{
  const YAML::Node negativeValuesConfig = YAML::Load(R"(
        simulation-params:
          length: -64
          n-fish: -1000
          max-steps: -1000
          delta-t: -0.01
          snapshot-interval: -100
        fish-params:
          vel-standard: -1.5
          vel-repulsion: -1.5
          vel-escape: -7.5
          body-length: -1.0
          repulsion-radius: -1.0
          attraction-radius: -7.5
          n-cog: -3
          attraction-strength: -15.0
          attraction-duration: -0.1
    )");

  EXPECT_EQ(negativeValuesConfig >> sim_param, EXIT_FAILURE);
  EXPECT_EQ(negativeValuesConfig >> fish_param, EXIT_FAILURE);
}
