#include "eom.hpp"
#include "coordinate.hpp"
#include "fish.hpp"
#include "simulation.hpp"

Vect3 calcDeltaVRepulsion(const Fish &fish,
  const Fish &other_fish,
  const SimParam &sim_param,
  const FishParam &fish_param)
{
  Vect3 delta_v_repulsion{ 0.0, 0.0, 0.0 };

  // Orientational interaction
  delta_v_repulsion +=
    g(abs(vect12(fish.getPosition(), other_fish.getPosition(), sim_param.length)), fish_param.body_length)
    * vect12(fish.getVelocity(), other_fish.getVelocity(), sim_param.length);

  // Repulsion interaction
  delta_v_repulsion +=
    g(abs(vect12(fish.getPosition(), other_fish.getPosition(), sim_param.length)), fish_param.body_length)
    * (fish_param.vel_repulsion / abs(vect12(fish.getPosition(), other_fish.getPosition(), sim_param.length))
         * vect12(other_fish.getPosition(), fish.getPosition(), sim_param.length)
       - fish.getVelocity());

  return delta_v_repulsion;
}

Vect3 calcDeltaVAttraction(const Fish &fish,
  const Fish &other_fish,
  const SimParam &sim_param,
  const FishParam &fish_param)
{
  Vect3 delta_v_attraction{ 0.0, 0.0, 0.0 };

  // Attraction interaction
  delta_v_attraction +=
    (fish_param.vel_escape / abs(vect12(fish.getPosition(), other_fish.getPosition(), sim_param.length)))
      * vect12(fish.getPosition(), other_fish.getPosition(), sim_param.length)
    - fish.getVelocity();

  return delta_v_attraction;
}

double g(double distance, double body_length) { return distance <= body_length ? body_length / distance : 1.; }

Vect3 calcSelfPropulsion(const Fish &fish, const FishParam &fish_param)
{
  return (fish_param.vel_standard / fish.speed() - 1) * fish.getVelocity();
}


std::tuple<Vect3, unsigned int> calcRepulsion(const Fish &fish,
  const SimParam &sim_param,
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

  // Create and sort the fish in the inner cells
  std::vector<Fish *> inner_fish_ptr{};
  for (const auto &inner_cell_relpos : repulsion_inner) {
    int loop_x = static_cast<int>(center_x) + inner_cell_relpos[0];
    int loop_y = static_cast<int>(center_y) + inner_cell_relpos[1];
    int loop_z = static_cast<int>(center_z) + inner_cell_relpos[2];

    // Account for the periodic boundary conditions
    loop_x = (loop_x + static_cast<int>(cells.size())) % static_cast<int>(cells.size());
    loop_y = (loop_y + static_cast<int>(cells[0].size())) % static_cast<int>(cells[0].size());
    loop_z = (loop_z + static_cast<int>(cells[0][0].size())) % static_cast<int>(cells[0][0].size());

    assert(loop_x >= 0);
    assert(loop_y >= 0);
    assert(loop_z >= 0);

    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Skip the fish itself
      if (neighbour_fish_ptr == &fish) { continue; }

      inner_fish_ptr.push_back(neighbour_fish_ptr);
    }
  }

  // Sort the fish in the inner cells based on the distance to the fish
  std::sort(inner_fish_ptr.begin(), inner_fish_ptr.end(), [&fish, &sim_param](Fish *a, Fish *b) {
    return abs(vect12(fish.getPosition(), a->getPosition(), sim_param.length))
           < abs(vect12(fish.getPosition(), b->getPosition(), sim_param.length));
  });


  // Calculate the repulsion with up to n_cog nearest fish
  for (unsigned long i = 0; i < std::min(static_cast<unsigned long>(fish_param.n_cog), inner_fish_ptr.size()); i++) {
    delta_v_repulsion += calcDeltaVRepulsion(fish, *inner_fish_ptr[i], sim_param, fish_param);
    neighbour_count++;
  }

  // If already enough fish are found, return the result
  if (neighbour_count >= fish_param.n_cog) { return { delta_v_repulsion / neighbour_count, neighbour_count }; }


  // Pointer to the fish in the repulsion boundary cells
  std::vector<Fish *> boundary_fish_ptr{};
  for (const auto &boundary_cell_relpos : repulsion_boundary) {
    int loop_x = static_cast<int>(center_x) + boundary_cell_relpos[0];
    int loop_y = static_cast<int>(center_y) + boundary_cell_relpos[1];
    int loop_z = static_cast<int>(center_z) + boundary_cell_relpos[2];
    // Account for the periodic boundary conditions
    loop_x = (loop_x + static_cast<int>(cells.size())) % static_cast<int>(cells.size());
    loop_y = (loop_y + static_cast<int>(cells[0].size())) % static_cast<int>(cells[0].size());
    loop_z = (loop_z + static_cast<int>(cells[0][0].size())) % static_cast<int>(cells[0][0].size());

    assert(loop_x >= 0);
    assert(loop_y >= 0);
    assert(loop_z >= 0);

    // Loop through the fish in the neighboring cell
    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Skip the fish itself
      if (neighbour_fish_ptr == &fish) { continue; }

      // Check if the fish is within the repulsion radius
      if (abs(vect12(fish.getPosition(), neighbour_fish_ptr->getPosition(), sim_param.length))
          > fish_param.repulsion_radius) {
        continue;
      }
      boundary_fish_ptr.push_back(neighbour_fish_ptr);
    }
  }

  // Sort the fish in the boundary cells based on the distance to the fish
  std::sort(boundary_fish_ptr.begin(), boundary_fish_ptr.end(), [&fish, &sim_param](Fish *a, Fish *b) {
    return abs(vect12(fish.getPosition(), a->getPosition(), sim_param.length))
           < abs(vect12(fish.getPosition(), b->getPosition(), sim_param.length));
  });

  // Calculate the repulsion with up to n_cog - neighbour_count nearest fish
  unsigned long inner_fish_size = neighbour_count;
  for (unsigned long i = 0;
       i < std::min(static_cast<unsigned long>(fish_param.n_cog - inner_fish_size), boundary_fish_ptr.size());
       i++) {
    delta_v_repulsion += calcDeltaVRepulsion(fish, *boundary_fish_ptr[i], sim_param, fish_param);
    neighbour_count++;
  }

  return { neighbour_count != 0 ? delta_v_repulsion / neighbour_count : Vect3{ 0.0, 0.0, 0.0 }, neighbour_count };
}

std::tuple<Vect3, unsigned int> calcAttraction(const Fish &fish,
  const SimParam &sim_param,
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
    int loop_x = static_cast<int>(center_x) + boundary_cell_relpos[0];
    int loop_y = static_cast<int>(center_y) + boundary_cell_relpos[1];
    int loop_z = static_cast<int>(center_z) + boundary_cell_relpos[2];

    // Account for the periodic boundary conditions
    loop_x = (loop_x + static_cast<int>(sim_param.length)) % static_cast<int>(sim_param.length);
    loop_y = (loop_y + static_cast<int>(sim_param.length)) % static_cast<int>(sim_param.length);
    loop_z = (loop_z + static_cast<int>(sim_param.length)) % static_cast<int>(sim_param.length);


    // Loop through the fish in the neighboring cell
    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Skip the fish itself
      if (neighbour_fish_ptr == &fish) { continue; }
      // Check if the fish is within the attraction radius
      if (abs(vect12(fish.getPosition(), neighbour_fish_ptr->getPosition(), sim_param.length))
          > fish_param.attraction_radius) {
        continue;
      } else if (abs(vect12(fish.getPosition(), neighbour_fish_ptr->getPosition(), sim_param.length))
                 < fish_param.repulsion_radius) {
        continue;
      }

      // Attraction interaction
      delta_v_attraction += calcDeltaVAttraction(fish, *neighbour_fish_ptr, sim_param, fish_param);

      neighbour_count++;
    }
  }

  // Loop through the neighboring inner cells
  for (const auto &inner_cell_relpos : attractive_inner) {
    int loop_x = static_cast<int>(center_x) + inner_cell_relpos[0];
    int loop_y = static_cast<int>(center_y) + inner_cell_relpos[1];
    int loop_z = static_cast<int>(center_z) + inner_cell_relpos[2];

    // Account for the periodic boundary conditions
    loop_x = (loop_x + static_cast<int>(sim_param.length)) % static_cast<int>(sim_param.length);
    loop_y = (loop_y + static_cast<int>(sim_param.length)) % static_cast<int>(sim_param.length);
    loop_z = (loop_z + static_cast<int>(sim_param.length)) % static_cast<int>(sim_param.length);


    // Loop through the fish in the neighboring cell
    for (auto *neighbour_fish_ptr : cells[loop_x][loop_y][loop_z]) {
      // Skip the fish itself
      if (neighbour_fish_ptr == &fish) { continue; }
      // Attraction interaction
      delta_v_attraction += calcDeltaVAttraction(fish, *neighbour_fish_ptr, sim_param, fish_param);

      neighbour_count++;
    }
  }

  return { neighbour_count != 0 ? fish.getLambda() * delta_v_attraction / neighbour_count : Vect3{ 0.0, 0.0, 0.0 },
    neighbour_count };
}