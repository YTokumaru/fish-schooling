#include "io.hpp"
#include "yaml-cpp/exceptions.h"

int operator>>(const YAML::Node &node, SimParam &param)
{
  try {
    YAML::Node sim_params = node["sim-params"];
    param.length = sim_params["length"].as<unsigned int>();
    param.n_fish = sim_params["n-fish"].as<unsigned int>();
    param.max_steps = sim_params["max-steps"].as<unsigned int>();
    param.delta_t = sim_params["delta-t"].as<double>();
  } catch (YAML::Exception &e) {
    std::cerr << "Error while reading from file: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int operator>>(const YAML::Node &node, FishParam &param)
{
  try {
    YAML::Node fish_params = node["fish-params"];
    param.vel_standard = fish_params["vel-standard"].as<double>();
    param.vel_repulsion = fish_params["vel-repulsion"].as<double>();
    param.vel_escape = fish_params["vel-escape"].as<double>();
    param.body_length = fish_params["body-length"].as<double>();
    param.repulsion_radius = fish_params["repulsion-radius"].as<double>();
    param.attraction_radius = fish_params["attraction-radius"].as<double>();
    param.n_cog = fish_params["n-cog"].as<unsigned int>();
    param.attraction_str = fish_params["attraction-strength"].as<double>();
    param.attraction_duration = fish_params["attraction-duration"].as<double>();
  } catch (YAML::Exception &e) {
    std::cerr << "Error while reading form file: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

std::tuple<SimParam, FishParam> loadConfig(std::string filename)
{
  YAML::Node config = YAML::LoadFile(filename);
  SimParam sim_param;
  FishParam fish_param;
  config >> sim_param;
  config >> fish_param;
  return std::make_tuple(sim_param, fish_param);
}
