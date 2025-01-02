#ifndef FISH_HPP
#define FISH_HPP

#include "coordinate.hpp"
#include "simulation.hpp"
#include <array>
#include <cmath>

class Fish
{
private:
  Vect3 position;
  Vect3 velocity;
  Vect3 delta_velocity;
  double lambda;

public:
  Fish();
  Fish(Vect3 position, Vect3 velocity, Vect3 delta_velocity, double lambda);
  Fish(double x,
    double y,
    double z,
    double vx,
    double vy,
    double vz,
    double delta_vx,
    double delta_vy,
    double delta_vz,
    double lambda);
  ~Fish();
  void update(double delta_t, unsigned int len, double dldt);
  void update(SimParam sim_param, FishParam fish_param);
  void setLambda(double lambda);
  void setPosition(double x, double y, double z);
  void setVelocity(double vx, double vy, double vz);
  void setVelocity(Vect3 velocity);
  void setDeltaVelocity(double delta_vx, double delta_vy, double delta_vz);
  void setDeltaVelocity(Vect3 delta_velocity);
  [[nodiscard]] inline Vect3 getPosition() const { return position; }
  [[nodiscard]] inline Vect3 getVelocity() const { return velocity; }
  [[nodiscard]] inline Vect3 getDeltaVelocity() const { return delta_velocity; }
  [[nodiscard]] inline double getLambda() const { return lambda; }
  [[nodiscard]] double speed() const;
};

#endif// FISH_HPP