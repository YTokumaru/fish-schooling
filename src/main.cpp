#include "coordinate.hpp"
#include "eom.hpp"
#include "fish.hpp"
#include "io.hpp"
#include "simulation.hpp"
#include <argparse/argparse.hpp>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <string>


int main(int argc, char *argv[])
{
  // Parse the command line arguments
  argparse::ArgumentParser program("fish_schooling");

  if (parseArguments(argc, argv, program) == EXIT_FAILURE) {
    std::cout << program;
    return 1;
  }


  // Load the parameters from the YAML file
  YAML::Node config = YAML::LoadFile(program.get<std::string>("--config"));

  FishParam fish_param{};
  SimParam sim_param{};

  if ((config >> fish_param) == EXIT_FAILURE) {
    std::cerr << "Error reading fish parameters" << std::endl;
    return 1;
  }
  if ((config >> sim_param) == EXIT_FAILURE) {
    std::cerr << "Error reading simulation parameters" << std::endl;
    return 1;
  }

  // Output file
  std::ofstream output_file("output.txt");

  // Initialize fish with random positions and velocities
  std::random_device rand;
  std::mt19937 gen(rand());
  std::uniform_real_distribution<double> dis_pos(0.0, sim_param.length);
  std::uniform_real_distribution<double> dis_vel(-fish_param.vel_standard, fish_param.vel_standard);

  std::vector<Fish> fish(sim_param.n_fish, Fish{});
  for (auto &one_fish : fish) {
    one_fish.setPosition({ dis_pos(gen), dis_pos(gen), dis_pos(gen) });
    one_fish.setVelocity({ dis_vel(gen), dis_vel(gen), dis_vel(gen) });
  }

  // Pre-generate the relative positions of the neighboring cells
  auto repulsion_boundary = getBoundaryCells(fish_param.repulsion_radius);
  auto repulsion_inner = getInnerCells(fish_param.repulsion_radius);

  auto attractive_boundary = getBoundaryBetween(fish_param.repulsion_radius, fish_param.attraction_radius);
  auto attractive_inner = getInnerBetween(fish_param.repulsion_radius, fish_param.attraction_radius);

  // Main loop
  for (unsigned int time_step = 0; time_step < sim_param.max_steps; time_step++) {

    std::cout << "Time step: " << time_step << std::endl;

    // Sort the fish into 1x1x1 grid cells
    std::vector<std::vector<std::vector<std::vector<Fish *>>>> cells(sim_param.length,
      std::vector<std::vector<std::vector<Fish *>>>(
        sim_param.length, std::vector<std::vector<Fish *>>(sim_param.length)));
    for (auto &one_fish : fish) {
      auto [x_pos, y_pos, z_pos] = one_fish.getPosition();
      auto x = static_cast<unsigned long>(x_pos);
      auto y = static_cast<unsigned long>(y_pos);
      auto z = static_cast<unsigned long>(z_pos);

      cells[x][y][z].push_back(&one_fish);
    }

// Loop over the fish and store the delta velocity
#pragma omp parallel for default(none), \
  shared(                               \
    fish, sim_param, fish_param, cells, repulsion_boundary, repulsion_inner, attractive_boundary, attractive_inner)
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


    // Update the fish positions and velocities
#pragma omp parallel for default(none), shared(fish, sim_param, fish_param)
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