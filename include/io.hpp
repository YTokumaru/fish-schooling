#ifndef IO_HPP
#define IO_HPP

#include "simulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

// TODO: We might need to validate/warn about the parameters:
// n_fish > 0
// delta_t > attraction_duration
// Perhaps make a validation function?

int operator>>(const YAML::Node &node, SimParam &param);

int operator>>(const YAML::Node &node, FishParam &param);

#endif// IO_HPP