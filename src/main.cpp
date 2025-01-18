#include "coordinate.hpp"
#include "eom.hpp"
#include "fish.hpp"
#include "io.hpp"
#include "simulation.hpp"
#include <argparse/argparse.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <string>
#include <vector>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

int main(int argc, char *argv[])
{
  // Parse the command line arguments
  argparse::ArgumentParser program("fish_schooling");

  if (parseArguments(argc, argv, program) == EXIT_FAILURE) {
    std::cout << program;
    return 1;
  }


  // Load the parameters from the YAML file
  const YAML::Node config = YAML::LoadFile(program.get<std::string>("--config"));

  FishParam fish_param{};
  SimParam sim_param{};

  if ((config >> fish_param) == EXIT_FAILURE) {
    std::cerr << "Error reading fish parameters" << '\n';
    return 1;
  }
  if ((config >> sim_param) == EXIT_FAILURE) {
    std::cerr << "Error reading simulation parameters" << '\n';
    return 1;
  }

  // Output file
  std::ofstream output_file("output.txt");

  // Initialize fish with random positions and velocities
  // std::random_device rand;
  // std::mt19937 gen(rand());
  // std::uniform_real_distribution<double> dis_pos(0.0, sim_param.length);
  // std::uniform_real_distribution<double> dis_vel(-fish_param.vel_standard, fish_param.vel_standard);

  // std::vector<Fish> fish(sim_param.n_fish, Fish{});
  // for (auto &one_fish : fish) {
  //   one_fish.setPosition({ .x = dis_pos(gen), .y = dis_pos(gen), .z = dis_pos(gen) });
  //   one_fish.setVelocity({ .x = dis_vel(gen), .y = dis_vel(gen), .z = dis_vel(gen) });
  // }

  // Initialize fish with a shere
  std::random_device rand;
  std::mt19937 gen(rand());
  const double init_r = fish_param.repulsion_radius * std::cbrt(sim_param.n_fish);
  std::uniform_real_distribution<double> dis_r(0.0, init_r);
  std::uniform_real_distribution<double> dis_theta(0.0, 2 * M_PI);
  std::uniform_real_distribution<double> dis_phi(0.0, M_PI);

  std::vector<Fish> fish(sim_param.n_fish, Fish{});
  for (auto &one_fish : fish) {
    const double r = dis_r(gen);
    const double theta = dis_theta(gen);
    const double phi = dis_phi(gen);
    one_fish.setPosition({ .x = r * std::sin(phi) * std::cos(theta) + static_cast<double>(sim_param.length) / 2,
      .y = r * std::sin(phi) * std::sin(theta) + static_cast<double>(sim_param.length) / 2,
      .z = r * std::cos(phi) + static_cast<double>(sim_param.length) / 2 });
    one_fish.setVelocity({ .x = fish_param.vel_standard, .y = 0, .z = 0 });
  }

  // Pre-generate the relative positions of the neighboring cells
  const auto repulsion_boundary = getBoundaryCells(fish_param.repulsion_radius);
  const auto repulsion_inner = getInnerCells(fish_param.repulsion_radius);

  const auto attractive_boundary = getBoundaryBetween(fish_param.repulsion_radius, fish_param.attraction_radius);
  const auto attractive_inner = getInnerBetween(fish_param.repulsion_radius, fish_param.attraction_radius);

  // Main loop
  for (unsigned int time_step = 0; time_step < sim_param.max_steps; time_step++) {


    std::cout << "Time step: " << time_step << '\n';

    std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
      std::vector<std::vector<std::vector<Fish *>>>(
        sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));

    // Sort the fish into 1x1x1 grid cells
    for (auto &one_fish : fish) {
      auto [x_pos, y_pos, z_pos] = one_fish.getPosition();
      auto x = static_cast<unsigned long>(x_pos);
      auto y = static_cast<unsigned long>(y_pos);
      auto z = static_cast<unsigned long>(z_pos);

      cells[x][y][z].push_back(&one_fish);
    }

// Loop over the fish and store the delta velocity
#pragma omp parallel default(none) shared(cells, fish), \
  firstprivate(fish_param, sim_param, repulsion_boundary, repulsion_inner, attractive_boundary, attractive_inner)
    {
#pragma omp for schedule(static)
      for (auto &one_fish : fish) {

        // Calculate the self-propulsion
        auto delta_v_self = calcSelfPropulsion(one_fish, fish_param);

        auto [delta_v_repulsion, n_fish_repulsion] =
          calcRepulsion(one_fish, sim_param, fish_param, cells, repulsion_boundary, repulsion_inner);

        if (n_fish_repulsion < fish_param.n_cog) { one_fish.setLambda(fish_param.attraction_str); }

        if (one_fish.getLambda() > 0) {
          auto [delta_v_attraction, n_fish_attrac] =
            calcAttraction(one_fish, sim_param, fish_param, cells, attractive_boundary, attractive_inner);

          one_fish.setDeltaVelocity(delta_v_self + delta_v_repulsion + delta_v_attraction);
        } else {
          one_fish.setDeltaVelocity(delta_v_self + delta_v_repulsion);
        }
      }
    }

    // Update the fish positions and velocities
    for (auto &one_fish : fish) { one_fish.update(sim_param, fish_param); }

    if (time_step % sim_param.snapshot_interval == 0) {
      // Output the fish positions
      for (auto &one_fish : fish) {
        auto [x, y, z] = one_fish.getPosition();
        auto [vx, vy, vz] = one_fish.getVelocity();
        output_file << x << " " << y << " " << z << " " << vx << " " << vy << " " << vz << '\n';
      }
    }
  }

  output_file.close();
  return EXIT_SUCCESS;
}
