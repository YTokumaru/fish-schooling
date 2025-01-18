#include "fish.hpp"

#include "coordinate.hpp"
#include "simulation.hpp"

Fish::Fish()
  : m_position({ .x = 0, .y = 0, .z = 0 }), m_velocity({ .x = 0, .y = 0, .z = 0 }),
    m_delta_velocity({ .x = 0, .y = 0, .z = 0 }), m_lambda(0)
{}

Fish::Fish(Vect3 position, Vect3 velocity, Vect3 delta_velocity, double lambda)
  : m_position(position), m_velocity(velocity), m_delta_velocity(delta_velocity), m_lambda(lambda)
{}

void Fish::update(double delta_t, unsigned int len, double dldt)
{
  // TODO: Significant digits may be lost here
  m_velocity += m_delta_velocity * delta_t;

  // Reset the delta velocity
  m_delta_velocity = { .x = 0, .y = 0, .z = 0 };

  m_position += m_velocity * delta_t;

  // Account for the periodic boundary conditions
  m_position = periodic(m_position, len);

  m_lambda - dldt *delta_t > 0 ? m_lambda -= dldt *delta_t : m_lambda = 0.0;
}

void Fish::update(SimParam sim_param, FishParam fish_param)
{
  update(sim_param.delta_t, sim_param.length, fish_param.attraction_str / fish_param.attraction_duration);
}

double Fish::speed() const { return absolute(m_velocity); }

void Fish::setPosition(double x, double y, double z) { m_position = { .x = x, .y = y, .z = z }; }

void Fish::setPosition(Vect3 position) { m_position = position; }

void Fish::setLambda(double lambda) { m_lambda = lambda; }

void Fish::setVelocity(double vx, double vy, double vz) { m_velocity = { .x = vx, .y = vy, .z = vz }; }

void Fish::setVelocity(Vect3 velocity) { m_velocity = velocity; }

void Fish::setDeltaVelocity(double delta_vx, double delta_vy, double delta_vz)
{
  m_delta_velocity = { .x = delta_vx, .y = delta_vy, .z = delta_vz };
}

void Fish::setDeltaVelocity(Vect3 delta_velocity) { m_delta_velocity = delta_velocity; }
