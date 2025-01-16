#ifndef FISH_HPP
#define FISH_HPP

#include "coordinate.hpp"
#include "simulation.hpp"
#include <array>
#include <cmath>

class Fish
{
private:
  Vect3 m_position;
  Vect3 m_velocity;
  Vect3 m_delta_velocity;
  double m_lambda;

public:
  Fish();
  Fish(Vect3 position, Vect3 velocity, Vect3 delta_velocity, double lambda);
  ~Fish() = default;
  void update(double delta_t, unsigned int len, double dldt);
  void update(SimParam sim_param, FishParam fish_param);
  void setLambda(double lambda);
  void setPosition(double x, double y, double z);
  void setPosition(Vect3 position);
  void setVelocity(double vx, double vy, double vz);
  void setVelocity(Vect3 velocity);
  void setDeltaVelocity(double delta_vx, double delta_vy, double delta_vz);
  void setDeltaVelocity(Vect3 delta_velocity);
  [[nodiscard]] inline Vect3 getPosition() const { return m_position; }
  [[nodiscard]] inline Vect3 getVelocity() const { return m_velocity; }
  [[nodiscard]] inline Vect3 getDeltaVelocity() const { return m_delta_velocity; }
  [[nodiscard]] inline double getLambda() const { return m_lambda; }
  [[nodiscard]] double speed() const;
};

#endif// FISH_HPP