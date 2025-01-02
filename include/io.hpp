#ifndef IO_HPP
#define IO_HPP

#include "simulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>


int operator>>(const YAML::Node &node, SimParam &param);

int operator>>(const YAML::Node &node, FishParam &param);

#endif// IO_HPP