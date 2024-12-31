#include <iostream>
#include <array>
#include <random>
#include <fstream>
#include "fish.hpp"
#include "eom.hpp"
#include "coordinate.hpp"

int main() {
  // Output file
  std::ofstream file("output.txt");

  constexpr unsigned int LEN = 16;
  constexpr double RHO = 3;
  constexpr unsigned int N = static_cast<unsigned int>(RHO * LEN * LEN * LEN);
  constexpr double LAMBDA = 10.0;
  constexpr double TAU = 0.1;
  constexpr double DLDT = LAMBDA / TAU;
  constexpr double V_0 = 1.5;
  constexpr double V_R = 1.5;
  constexpr double V_A = 7.5;
  constexpr unsigned int N_U = 3;
  constexpr double BODY_LENGTH = 1.0;
  constexpr double REPULSION_RADIUS = 1.5;
  constexpr double ATTRACTIVE_RADIUS = 5.0;
  

  

  const unsigned int N_STEPS = 1000;
  const double DT = 0.01;

  std::array<Fish, N> fish{};

  // Initialize fish with random positions and velocities
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis_pos(0.0, LEN);
  std::uniform_real_distribution<double> dis_vel(-V_0, V_0);
  for (int i = 0; i < N; i++) {
    fish[i].setPosition(dis_pos(gen), dis_pos(gen), dis_pos(gen));
    fish[i].setVelocity(dis_vel(gen), dis_vel(gen), dis_vel(gen));
  }
  
  // Pre-generate the relative positions of the neighboring cells
  auto repulsion_boundary = getBoundaryCells(REPULSION_RADIUS);
  auto repulsion_inner = getInnerCells(REPULSION_RADIUS);

  auto attractive_boundary = getBoundaryBetween(REPULSION_RADIUS, ATTRACTIVE_RADIUS);
  auto attractive_inner = getInnerBetween(REPULSION_RADIUS, ATTRACTIVE_RADIUS);

  // Main loop
  for (int t = 0; t < N_STEPS; t++)
  {
    // Sort the fish into 1x1x1 grid cells
    std::vector<std::vector<std::vector<std::vector<Fish*>>>> cells(LEN, std::vector<std::vector<std::vector<Fish*>>>(LEN, std::vector<std::vector<Fish*>>(LEN)));
    
    // Initialize the cells
    for (auto one_fish: fish)
    {
      int x = one_fish.getX();
      int y = one_fish.getY();
      int z = one_fish.getZ();

      cells[x][y][z].push_back(&one_fish);
    }
    
    // Loop over the cells
    for (size_t x = 0; x < LEN; x++)
    {
      for (size_t y = 0; y < LEN; y++)
      {
        for (size_t z = 0; z < LEN; z++)
        {
          // Loop over the fish in the cell
          for (size_t i = 0; i < cells[x][y][z].size(); i++)
          {
            double delta_vx = 0.0;
            double delta_vy = 0.0;
            double delta_vz = 0.0;

            // Add the self-propulsion term
            double speed = cells[x][y][z][i]->speed();
            delta_vx += (V_0 / speed - 1) * cells[x][y][z][i]->getVx();
            delta_vy += (V_0 / speed - 1) * cells[x][y][z][i]->getVy();
            delta_vz += (V_0 / speed - 1) * cells[x][y][z][i]->getVz();

            // Add the orientation term
            double delta_vx_orientation = 0.0;
            double delta_vy_orientation = 0.0;
            double delta_vz_orientation = 0.0;

            // The repulsion term
            double delta_vx_repulsion = 0.0;
            double delta_vy_repulsion = 0.0;
            double delta_vz_repulsion = 0.0;

            int fish_in_re = 0;

            // Loop through the repulsion cells
            for(auto rel_coor: repulsion_boundary)
            {
              int x_rel = x + rel_coor[0];
              int y_rel = y + rel_coor[1];
              int z_rel = z + rel_coor[2];

              // Account for the periodic boundary conditions
              x_rel = (x_rel + LEN) % LEN;
              y_rel = (y_rel + LEN) % LEN;
              z_rel = (z_rel + LEN) % LEN;

              // Loop through the fish in the neighboring cell
              for (Fish* other_fish_ptr: cells[x_rel][y_rel][z_rel])
              {
                // Check if the fish is within the repulsion radius
                double rel_distance = distance(*cells[x][y][z][i], *other_fish_ptr);
                if (rel_distance > REPULSION_RADIUS) continue;

                // Add the orientation term
                delta_vx_orientation += g(rel_distance, BODY_LENGTH) * (other_fish_ptr->getVx() - cells[x][y][z][i]->getVx());
                delta_vy_orientation += g(rel_distance, BODY_LENGTH) * (other_fish_ptr->getVy() - cells[x][y][z][i]->getVy());
                delta_vz_orientation += g(rel_distance, BODY_LENGTH) * (other_fish_ptr->getVz() - cells[x][y][z][i]->getVz());
                
                // Add the repulsion term
                delta_vx_repulsion += g(rel_distance, BODY_LENGTH) * 
                                      (V_R * (other_fish_ptr->getX() - cells[x][y][z][i]->getX()) / rel_distance - cells[x][y][z][i]->getVx());
                delta_vy_repulsion += g(rel_distance, BODY_LENGTH) *
                                      (V_R * (other_fish_ptr->getY() - cells[x][y][z][i]->getY()) / rel_distance - cells[x][y][z][i]->getVy());
                delta_vz_repulsion += g(rel_distance, BODY_LENGTH) *
                                      (V_R * (other_fish_ptr->getZ() - cells[x][y][z][i]->getZ()) / rel_distance - cells[x][y][z][i]->getVz());

                fish_in_re++;
              }

            }
            
            // Loop through the inner repulsion cells
            for(auto rel_coor: repulsion_inner)
            {
              int x_rel = x + rel_coor[0];
              int y_rel = y + rel_coor[1];
              int z_rel = z + rel_coor[2];

              // Account for the periodic boundary conditions
              x_rel = (x_rel + LEN) % LEN;
              y_rel = (y_rel + LEN) % LEN;
              z_rel = (z_rel + LEN) % LEN;

              // Loop through the fish in the neighboring cell
              for (Fish* other_fish_ptr: cells[x_rel][y_rel][z_rel])
              {
                // Check if the fish is within the repulsion radius not necessary
                double rel_distance = distance(*cells[x][y][z][i], *other_fish_ptr);
                // if (rel_distance > REPULSION_RADIUS) continue;

                // Add the orientation term
                delta_vx_orientation += g(rel_distance, BODY_LENGTH) * (other_fish_ptr->getVx() - cells[x][y][z][i]->getVx());
                delta_vy_orientation += g(rel_distance, BODY_LENGTH) * (other_fish_ptr->getVy() - cells[x][y][z][i]->getVy());
                delta_vz_orientation += g(rel_distance, BODY_LENGTH) * (other_fish_ptr->getVz() - cells[x][y][z][i]->getVz());
                
                // Add the repulsion term
                delta_vx_repulsion += g(rel_distance, BODY_LENGTH) * 
                                      (V_R * (other_fish_ptr->getX() - cells[x][y][z][i]->getX()) / rel_distance - cells[x][y][z][i]->getVx());
                delta_vy_repulsion += g(rel_distance, BODY_LENGTH) *
                                      (V_R * (other_fish_ptr->getY() - cells[x][y][z][i]->getY()) / rel_distance - cells[x][y][z][i]->getVy());
                delta_vz_repulsion += g(rel_distance, BODY_LENGTH) *
                                      (V_R * (other_fish_ptr->getZ() - cells[x][y][z][i]->getZ()) / rel_distance - cells[x][y][z][i]->getVz());

                fish_in_re++;
              }
            }

            // Normalize the term
            if (fish_in_re > 0)
            {
              delta_vx_orientation /= fish_in_re;
              delta_vy_orientation /= fish_in_re;
              delta_vz_orientation /= fish_in_re;
              
              delta_vx_repulsion /= fish_in_re;
              delta_vy_repulsion /= fish_in_re;
              delta_vz_repulsion /= fish_in_re;
            }
            
            delta_vx += delta_vx_orientation + delta_vx_repulsion;
            delta_vy += delta_vy_orientation + delta_vy_repulsion;
            delta_vz += delta_vz_orientation + delta_vz_repulsion;

            if(fish_in_re <= N_U)
            {
              cells[x][y][z][i]->setLambda(LAMBDA);
            }
            

            // Add the attraction term
            double delta_vx_attraction = 0.0;
            double delta_vy_attraction = 0.0;
            double delta_vz_attraction = 0.0;

            int fish_in_attr = 0;

            // Loop through the attractive cells

            for(auto rel_coor: attractive_boundary)
            {
              int x_rel = x + rel_coor[0];
              int y_rel = y + rel_coor[1];
              int z_rel = z + rel_coor[2];

              // Account for the periodic boundary conditions
              x_rel = (x_rel + LEN) % LEN;
              y_rel = (y_rel + LEN) % LEN;
              z_rel = (z_rel + LEN) % LEN;

              // Loop through the fish in the neighboring cell
              for (Fish* other_fish_ptr: cells[x_rel][y_rel][z_rel])
              {
                // Check if the fish is within the repulsion radius
                double rel_distance = distance(*cells[x][y][z][i], *other_fish_ptr);
                if (rel_distance > ATTRACTIVE_RADIUS || rel_distance < REPULSION_RADIUS) continue;

                // Add the attraction term
                delta_vx_attraction += (V_A * (cells[x][y][z][i]->getX() - other_fish_ptr->getX()) / rel_distance - cells[x][y][z][i]->getVx());
                delta_vy_attraction += (V_A * (cells[x][y][z][i]->getY() - other_fish_ptr->getY()) / rel_distance - cells[x][y][z][i]->getVy());
                delta_vz_attraction += (V_A * (cells[x][y][z][i]->getZ() - other_fish_ptr->getZ()) / rel_distance - cells[x][y][z][i]->getVz());

                fish_in_attr++;
              }
            }

            // Loop through the inner attractive cells
            for(auto rel_coor: attractive_inner)
            {
              int x_rel = x + rel_coor[0];
              int y_rel = y + rel_coor[1];
              int z_rel = z + rel_coor[2];

              // Account for the periodic boundary conditions
              x_rel = (x_rel + LEN) % LEN;
              y_rel = (y_rel + LEN) % LEN;
              z_rel = (z_rel + LEN) % LEN;

              // Loop through the fish in the neighboring cell
              for (Fish* other_fish_ptr: cells[x_rel][y_rel][z_rel])
              {
                // Check if the fish is within the repulsion radius not necessary
                double rel_distance = distance(*cells[x][y][z][i], *other_fish_ptr);
                // if (rel_distance > REPULSION_RADIUS) continue;

                // Add the attraction term
                delta_vx_attraction += (V_A * (cells[x][y][z][i]->getX() - other_fish_ptr->getX()) / rel_distance - cells[x][y][z][i]->getVx());
                delta_vy_attraction += (V_A * (cells[x][y][z][i]->getY() - other_fish_ptr->getY()) / rel_distance - cells[x][y][z][i]->getVy());
                delta_vz_attraction += (V_A * (cells[x][y][z][i]->getZ() - other_fish_ptr->getZ()) / rel_distance - cells[x][y][z][i]->getVz());

                fish_in_attr++;
              }
            }

            // Normalize the term
            if (fish_in_attr > 0)
            {
              delta_vx_attraction /= fish_in_attr;
              delta_vy_attraction /= fish_in_attr;
              delta_vz_attraction /= fish_in_attr;

              delta_vx_attraction *= cells[x][y][z][i]->getLambda();
              delta_vy_attraction *= cells[x][y][z][i]->getLambda();
              delta_vz_attraction *= cells[x][y][z][i]->getLambda();
            }
            
            delta_vx += delta_vx_attraction;
            delta_vy += delta_vy_attraction;
            delta_vz += delta_vz_attraction;

            // Update the velocity
            cells[x][y][z][i]->setDeltaVelocity(delta_vx, delta_vy, delta_vz);
          }
        }
      }
    }

    for (size_t i = 0; i < fish.size(); i++)
    {
      fish[i].update(DT, LEN);
    }
    
    // Output the positions of the fish to file
    for (size_t i = 0; i < fish.size(); i++)
    {
      file << fish[i].getX() << " " << fish[i].getY() << " " << fish[i].getZ() << " "
            << fish[i].getVx() << " " << fish[i].getVy() << " " << fish[i].getVz() << '\n';
    }
  }

  file.close();
  return 0;
}
