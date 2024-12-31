#ifndef FISH_HPP
#define FISH_HPP

#include <cmath>
class Fish
{
private:
  double x;
  double y;
  double z;
  double vx;
  double vy;
  double vz;
  double lambda;

  double delta_vx;
  double delta_vy;
  double delta_vz;
public:
  Fish();
  ~Fish();
  void update(double dt, int LEN);
  void setLambda(double lambda);
  void setPosition(double x, double y, double z);
  void setVelocity(double vx, double vy, double vz);
  void setDeltaVelocity(double delta_vx, double delta_vy, double delta_vz);
  inline double getX()const { return x; }
  inline double getY()const { return y; }
  inline double getZ()const { return z; }
  inline double getVx()const { return vx; }
  inline double getVy()const { return vy; }
  inline double getVz()const { return vz; }
  inline double getLambda()const { return lambda; }
  double speed()const;
};


double distance(Fish fish1, Fish fish2);

#endif // FISH_HPP