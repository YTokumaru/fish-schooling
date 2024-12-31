#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <vector>
#include <array>
#include <cmath>

std::vector<std::array<int, 3>> getBoundaryCells(double radius);

std::vector<std::array<int, 3>> getInnerCells(double radius);

std::vector<std::array<int, 3>> getBoundaryBetween(double radius1, double radius2);

std::vector<std::array<int, 3>> getInnerBetween(double radius1, double radius2);

#endif // COORDINATE_HPP