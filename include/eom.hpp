#ifndef EOM_CPP
#define EOM_CPP

#include "fish.hpp"
#include <cassert>
#include <tuple>
#include <vector>

double g(double distance, double body_length);

Vect3 calcSelfPropulsion(const Fish &fish, const FishParam &fish_param);

std::tuple<Vect3, unsigned int> calcRepulsion(const Fish &fish,
  const SimParam &sim_param,
  const FishParam &fish_param,
  const std::vector<std::vector<std::vector<std::vector<Fish *>>>> &cells,
  const std::vector<std::array<int, 3>> &repulsion_boundary,
  const std::vector<std::array<int, 3>> &repulsion_inner);

std::tuple<Vect3, unsigned int> calcAttraction(const Fish &fish,
  const SimParam &sim_param,
  const FishParam &fish_param,
  const std::vector<std::vector<std::vector<std::vector<Fish *>>>> &cells,
  const std::vector<std::array<int, 3>> &attractive_boundary,
  const std::vector<std::array<int, 3>> &attractive_inner);

#endif// EOM_CPP
