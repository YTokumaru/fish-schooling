#include "fish.hpp"

Fish::Fish(): x(0), y(0), z(0), vx(0), vy(0), vz(0), lambda(0), delta_vx(0), delta_vy(0), delta_vz(0)
{
}

Fish::~Fish()
{
}

void Fish::update(double dt, int LEN)
{
    vx += delta_vx;
    vy += delta_vy;
    vz += delta_vz;
 
    x += vx * dt;
    y += vy * dt;
    z += vz * dt;
    
    // Account for the periodic boundary conditions
    x = x < 0 ? x + LEN : x;
    x = x >= LEN ? x - LEN : x;
    y = y < 0 ? y + LEN : y;
    y = y >= LEN ? y - LEN : y;
    z = z < 0 ? z + LEN : z;
    z = z >= LEN ? z - LEN : z;

    if (lambda > 0)
    {
        lambda -= dt;
    }
    if (lambda < 0)
    {
        lambda = 0;
    }
}

double Fish::speed() const
{
    return std::sqrt(vx * vx + vy * vy + vz * vz);
}

void Fish::setPosition(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Fish::setLambda(double lambda)
{
    this->lambda = lambda;
}

void Fish::setVelocity(double vx, double vy, double vz)
{
    this->vx = vx;
    this->vy = vy;
    this->vz = vz;
}

void Fish::setDeltaVelocity(double delta_vx, double delta_vy, double delta_vz)
{
    this->delta_vx = delta_vx;
    this->delta_vy = delta_vy;
    this->delta_vz = delta_vz;
}

double distance(Fish fish1, Fish fish2)
{
    return std::sqrt((fish1.getX() - fish2.getX()) * (fish1.getX() - fish2.getX()) +
                     (fish1.getY() - fish2.getY()) * (fish1.getY() - fish2.getY()) +
                     (fish1.getZ() - fish2.getZ()) * (fish1.getZ() - fish2.getZ()));
}
