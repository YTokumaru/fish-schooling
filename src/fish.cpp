#include "fish.hpp"

Fish::Fish() : position({ 0, 0, 0 }), velocity({ 0, 0, 0 }), delta_velocity({ 0, 0, 0 }), lambda(0) {}

Fish::Fish(Vect3 position, Vect3 velocity, Vect3 delta_velocity, double lambda)
  : position(position), velocity(velocity), delta_velocity(delta_velocity), lambda(lambda)
{}

Fish::Fish(double x,
  double y,
  double z,
  double vx,
  double vy,
  double vz,
  double delta_vx,
  double delta_vy,
  double delta_vz,
  double lambda)
  : position({ x, y, z }), velocity({ vx, vy, vz }), delta_velocity({ delta_vx, delta_vy, delta_vz }), lambda(lambda)
{}

Fish::~Fish() {}

void Fish::update(double delta_t, unsigned int len, double dldt)
{
  // TODO: Significant digits may be lost here
  velocity += delta_velocity * delta_t;

  // Reset the delta velocity
  delta_velocity = { 0, 0, 0 };

  position += velocity * delta_t;

  // Account for the periodic boundary conditions
  position = periodize(position, len);

  lambda - dldt *delta_t > 0 ? lambda -= dldt *delta_t : lambda = 0.0;
}

void Fish::update(SimParam sim_param, FishParam fish_param)
{
  update(sim_param.delta_t, sim_param.length, fish_param.attraction_str / fish_param.attraction_duration);
}

double Fish::speed() const { return abs(velocity); }

void Fish::setPosition(double x, double y, double z) { position = { x, y, z }; }

void Fish::setPosition(Vect3 position) { this->position = position; }

void Fish::setLambda(double lambda) { this->lambda = lambda; }

void Fish::setVelocity(double vx, double vy, double vz) { velocity = { vx, vy, vz }; }

void Fish::setVelocity(Vect3 velocity) { this->velocity = velocity; }

void Fish::setDeltaVelocity(double delta_vx, double delta_vy, double delta_vz)
{
  delta_velocity = { delta_vx, delta_vy, delta_vz };
}

void Fish::setDeltaVelocity(Vect3 delta_velocity) { this->delta_velocity = delta_velocity; }
