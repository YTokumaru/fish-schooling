#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <array>
#include <cassert>
#include <cmath>
#include <vector>

struct Vect3
{
  double x;
  double y;
  double z;
};


Vect3 operator+(const Vect3 &lhs, const Vect3 &rhs);
Vect3 operator-(const Vect3 &lhs, const Vect3 &rhs);
Vect3 operator*(double scalar, const Vect3 &rhs);
Vect3 operator*(const Vect3 &lhs, double scalar);
Vect3 operator/(const Vect3 &lhs, double scalar);

Vect3 operator+=(Vect3 &lhs, const Vect3 &rhs);
Vect3 operator-=(Vect3 &lhs, const Vect3 &rhs);
Vect3 operator*=(Vect3 &lhs, double scalar);
Vect3 operator/=(Vect3 &lhs, double scalar);

Vect3 periodic(const Vect3 &vect, unsigned int len);

bool isCellOnBoundary(const std::array<int, 3> &cell, double radius, const Vect3 &center);

bool isCellInside(const std::array<int, 3> &cell, double radius, const Vect3 &center);

std::vector<std::array<int, 3>> getBoundaryCells(double radius, const Vect3 &center);

std::vector<std::array<int, 3>> getInnerCells(double radius, const Vect3 &center);

std::vector<std::array<int, 3>> getBoundaryCells(double radius);

std::vector<std::array<int, 3>> getInnerCells(double radius);

std::vector<std::array<int, 3>> getBoundaryBetween(double radius1, double radius2);

std::vector<std::array<int, 3>> getInnerBetween(double radius1, double radius2);

double abs(const Vect3 &vect);
Vect3 normalize(const Vect3 &vect);

Vect3 vect12(const Vect3 &vect1, const Vect3 &vect2);

Vect3 vect12(const Vect3 &vect1, Vect3 vect2, unsigned int len);

#endif// COORDINATE_HPP