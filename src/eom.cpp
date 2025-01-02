#include "eom.hpp"
#include "coordinate.hpp"
#include "fish.hpp"
#include "simulation.hpp"
#include <utility>

double g(double distance, double body_length) { return distance <= body_length ? body_length / distance : 1.; }

Vect3 calcSelfPropulsion(const Fish &fish, const FishParam &fish_param)
{
  return (fish_param.vel_standard / fish.speed() - 1) * fish.getVelocity();
}


std::tuple<Vect3, unsigned int> calcRepulsion(const Fish &fish,
  const FishParam &fish_param,
  const std::vector<std::vector<std::vector<std::vector<Fish *>>>> &cells,
  const std::vector<std::array<int, 3>> &repulsion_boundary,
  const std::vector<std::array<int, 3>> &repulsion_inner)
{
  Vect3 delta_v_repulsion{ 0.0, 0.0, 0.0 };
  unsigned int neighbour_count = 0;// Number of neighboring fish
  auto [fish_x, fish_y, fish_z] = fish.getPosition();
  auto center_x = static_cast<unsigned long>(fish_x);
  auto center_y = static_cast<unsigned long>(fish_y);
  auto center_z = static_cast<unsigned long>(fish_z);

  // Loop through the neighboring boundary cells
  for (const auto &boundary_cell_relpos : repulsion_boundary) {
    unsigned long loop_x = center_x + boundary_cell_relpos[0];
    unsigned long loop_y = center_y + boundary_cell_relpos[1];
    unsigned long loop_z = center_z + boundary_cell_relpos[2];
    // Account for the periodic boundary conditions
    loop_x = (loop_x + cells.size()) % cells.size();
    loop_y = (loop_y + cells[0].size()) % cells[0].size();
    loop_z = (loop_z + cells[0][0].size()) % cells[0][0].size();

    neighbour_count += cells[loop_x][loop_y][loop_z].size();

    // Loop through the fish in the neighboring cell
    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Check if the fish is within the repulsion radius
      if (abs(fish.getPosition() - neighbour_fish_ptr->getPosition()) > fish_param.repulsion_radius) { continue; }

      // Orientational interaction
      delta_v_repulsion += g(abs(fish.getPosition() - neighbour_fish_ptr->getPosition()), fish_param.body_length)
                           * vect12(fish.getVelocity(), neighbour_fish_ptr->getVelocity());

      // Repulsion interaction
      delta_v_repulsion += g(abs(fish.getPosition() - neighbour_fish_ptr->getPosition()), fish_param.body_length)
                           * (fish_param.vel_repulsion / abs(fish.getPosition() - neighbour_fish_ptr->getPosition())
                                * vect12(fish.getPosition(), neighbour_fish_ptr->getPosition())
                              - fish.getVelocity());
    }
  }

  // Loop through the neighboring inner cells
  for (const auto &inner_cell_relpos : repulsion_inner) {
    unsigned long loop_x = center_x + inner_cell_relpos[0];
    unsigned long loop_y = center_y + inner_cell_relpos[1];
    unsigned long loop_z = center_z + inner_cell_relpos[2];

    // Account for the periodic boundary conditions
    loop_x = (loop_x + cells.size()) % cells.size();
    loop_y = (loop_y + cells[0].size()) % cells[0].size();
    loop_z = (loop_z + cells[0][0].size()) % cells[0][0].size();

    neighbour_count += cells[loop_x][loop_y][loop_z].size();

    // Loop through the fish in the neighboring cell
    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Orientational interaction
      delta_v_repulsion += g(abs(fish.getPosition() - neighbour_fish_ptr->getPosition()), fish_param.body_length)
                           * vect12(fish.getVelocity(), neighbour_fish_ptr->getVelocity());

      // Repulsion interaction
      delta_v_repulsion += g(abs(fish.getPosition() - neighbour_fish_ptr->getPosition()), fish_param.body_length)
                           * (fish_param.vel_repulsion / abs(fish.getPosition() - neighbour_fish_ptr->getPosition())
                                * vect12(fish.getPosition(), neighbour_fish_ptr->getPosition())
                              - fish.getVelocity());
    }
  }

  return { delta_v_repulsion / neighbour_count, neighbour_count };
}

std::tuple<Vect3, unsigned int> calcAttraction(const Fish &fish,
  const FishParam &fish_param,
  const std::vector<std::vector<std::vector<std::vector<Fish *>>>> &cells,
  const std::vector<std::array<int, 3>> &attractive_boundary,
  const std::vector<std::array<int, 3>> &attractive_inner)
{
  Vect3 delta_v_attraction{ 0.0, 0.0, 0.0 };
  unsigned int neighbour_count = 0;// Number of neighboring fish
  auto [fish_x, fish_y, fish_z] = fish.getPosition();
  auto center_x = static_cast<unsigned long>(fish_x);
  auto center_y = static_cast<unsigned long>(fish_y);
  auto center_z = static_cast<unsigned long>(fish_z);

  // Loop through the neighboring boundary cells
  for (const auto &boundary_cell_relpos : attractive_boundary) {
    unsigned long loop_x = center_x + boundary_cell_relpos[0];
    unsigned long loop_y = center_y + boundary_cell_relpos[1];
    unsigned long loop_z = center_z + boundary_cell_relpos[2];

    // Account for the periodic boundary conditions
    loop_x = (loop_x + cells.size()) % cells.size();
    loop_y = (loop_y + cells[0].size()) % cells[0].size();
    loop_z = (loop_z + cells[0][0].size()) % cells[0][0].size();

    neighbour_count += cells[loop_x][loop_y][loop_z].size();

    // Loop through the fish in the neighboring cell
    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Check if the fish is within the attraction radius
      if (abs(fish.getPosition() - neighbour_fish_ptr->getPosition()) > fish_param.attraction_radius) {
        continue;
      } else if (abs(fish.getPosition() - neighbour_fish_ptr->getPosition()) < fish_param.repulsion_radius) {
        continue;
      }

      // Attraction interaction
      delta_v_attraction += (fish_param.vel_escape / abs(fish.getPosition() - neighbour_fish_ptr->getPosition()))
                              * vect12(neighbour_fish_ptr->getPosition(), fish.getPosition())
                            - fish.getVelocity();
    }
  }

  // Loop through the neighboring inner cells
  for (const auto &inner_cell_relpos : attractive_inner) {
    unsigned long loop_x = center_x + inner_cell_relpos[0];
    unsigned long loop_y = center_y + inner_cell_relpos[1];
    unsigned long loop_z = center_z + inner_cell_relpos[2];

    // Account for the periodic boundary conditions
    loop_x = (loop_x + cells.size()) % cells.size();
    loop_y = (loop_y + cells[0].size()) % cells[0].size();
    loop_z = (loop_z + cells[0][0].size()) % cells[0][0].size();

    neighbour_count += cells[loop_x][loop_y][loop_z].size();

    // Loop through the fish in the neighboring cell
    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Attraction interaction
      delta_v_attraction += (fish_param.vel_escape / abs(fish.getPosition() - neighbour_fish_ptr->getPosition()))
                              * vect12(neighbour_fish_ptr->getPosition(), fish.getPosition())
                            - fish.getVelocity();
    }
  }

  return { fish.getLambda() * delta_v_attraction / neighbour_count, neighbour_count };
}