#ifndef SIMULATION_HPP
#define SIMULATION_HPP


struct SimParam
{
  unsigned int length;
  unsigned int n_fish;
  unsigned int max_steps;
  double delta_t;
  unsigned int snapshot_interval;
};

struct FishParam
{
  double vel_standard;// The standard velocity of the fish
  double vel_repulsion;// The velocity of the fish when it avoids other fish
  double vel_escape;// The velocity of the fish when it escapes from the predator

  double body_length;// The body length of the fish
  double repulsion_radius;// The radius of the repulsion zone
  double attraction_radius;// The radius of the attraction zone
  unsigned int n_cog;// Max number of fish in the repulsion zone before the fish starts to escape
  double attraction_str;// The strength of the attraction force
  double attraction_duration;// The duration of the attraction force
};


#endif// SIMULATION_HPP
