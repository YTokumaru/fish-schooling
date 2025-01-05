#include "coordinate.hpp"

Vect3 operator+(const Vect3 &lhs, const Vect3 &rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }

Vect3 operator-(const Vect3 &lhs, const Vect3 &rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }

Vect3 operator*(double scalar, const Vect3 &rhs) { return { scalar * rhs.x, scalar * rhs.y, scalar * rhs.z }; }

Vect3 operator*(const Vect3 &lhs, double scalar) { return { scalar * lhs.x, scalar * lhs.y, scalar * lhs.z }; }

Vect3 operator/(const Vect3 &lhs, double scalar) { return { lhs.x / scalar, lhs.y / scalar, lhs.z / scalar }; }

Vect3 operator+=(Vect3 &lhs, const Vect3 &rhs)
{
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  lhs.z += rhs.z;
  return lhs;
}

Vect3 operator-=(Vect3 &lhs, const Vect3 &rhs)
{
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;
  lhs.z -= rhs.z;
  return lhs;
}

Vect3 operator*=(Vect3 &lhs, double scalar)
{
  lhs.x *= scalar;
  lhs.y *= scalar;
  lhs.z *= scalar;
  return lhs;
}

Vect3 operator/=(Vect3 &lhs, double scalar)
{
  lhs.x /= scalar;
  lhs.y /= scalar;
  lhs.z /= scalar;
  return lhs;
}

Vect3 periodize(const Vect3 &vect, unsigned int len)
{
  return { vect.x > len ? vect.x - len
           : vect.x < 0 ? vect.x + len
                        : vect.x,
    vect.y > len ? vect.y - len
    : vect.y < 0 ? vect.y + len
                 : vect.y,
    vect.z > len ? vect.z - len
    : vect.z < 0 ? vect.z + len
                 : vect.z };
}

double abs(const Vect3 &vect) { return std::sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z); }
Vect3 normalize(const Vect3 &vect)
{
  double magnitude = abs(vect);
  return { vect.x / magnitude, vect.y / magnitude, vect.z / magnitude };
}

Vect3 vect12(const Vect3 &vect1, const Vect3 &vect2)
{
  return { vect2.x - vect1.x, vect2.y - vect1.y, vect2.z - vect1.z };
}


std::vector<std::array<int, 3>> getBoundaryCells(double radius)
{
  if (radius <= 0) { return {}; }

  if (radius <= 0.5) { return { { 0, 0, 0 } }; }


  int cellRadius = static_cast<int>(radius) + 1;

  // Scan through the cells
  std::vector<std::array<int, 3>> boundaryCells{};
  for (int x = -cellRadius; x <= cellRadius; x++) {
    for (int y = -cellRadius; y <= cellRadius; y++) {
      for (int z = -cellRadius; z <= cellRadius; z++) {
        // Check if the cell is intersecting the sphere
        // Check distance with all vertices
        double vertex1 = std::sqrt((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) + (z - 0.5) * (z - 0.5));
        double vertex2 = std::sqrt((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) + (z + 0.5) * (z + 0.5));
        double vertex3 = std::sqrt((x - 0.5) * (x - 0.5) + (y + 0.5) * (y + 0.5) + (z - 0.5) * (z - 0.5));
        double vertex4 = std::sqrt((x - 0.5) * (x - 0.5) + (y + 0.5) * (y + 0.5) + (z + 0.5) * (z + 0.5));
        double vertex5 = std::sqrt((x + 0.5) * (x + 0.5) + (y - 0.5) * (y - 0.5) + (z - 0.5) * (z - 0.5));
        double vertex6 = std::sqrt((x + 0.5) * (x + 0.5) + (y - 0.5) * (y - 0.5) + (z + 0.5) * (z + 0.5));
        double vertex7 = std::sqrt((x + 0.5) * (x + 0.5) + (y + 0.5) * (y + 0.5) + (z - 0.5) * (z - 0.5));
        double vertex8 = std::sqrt((x + 0.5) * (x + 0.5) + (y + 0.5) * (y + 0.5) + (z + 0.5) * (z + 0.5));

        // Check distance with all faces
        double face1 = std::sqrt((x - 0.5) * (x - 0.5) + y * y + z * z);
        double face2 = std::sqrt((x + 0.5) * (x + 0.5) + y * y + z * z);
        double face3 = std::sqrt(x * x + (y - 0.5) * (y - 0.5) + z * z);
        double face4 = std::sqrt(x * x + (y + 0.5) * (y + 0.5) + z * z);
        double face5 = std::sqrt(x * x + y * y + (z - 0.5) * (z - 0.5));
        double face6 = std::sqrt(x * x + y * y + (z + 0.5) * (z + 0.5));

        // Check distance with all edges
        double edge1 = std::sqrt((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) + z * z);
        double edge2 = std::sqrt((x - 0.5) * (x - 0.5) + (y + 0.5) * (y + 0.5) + z * z);
        double edge3 = std::sqrt((x + 0.5) * (x + 0.5) + (y - 0.5) * (y - 0.5) + z * z);
        double edge4 = std::sqrt((x + 0.5) * (x + 0.5) + (y + 0.5) * (y + 0.5) + z * z);
        double edge5 = std::sqrt((x - 0.5) * (x - 0.5) + y * y + (z - 0.5) * (z - 0.5));
        double edge6 = std::sqrt((x - 0.5) * (x - 0.5) + y * y + (z + 0.5) * (z + 0.5));
        double edge7 = std::sqrt((x + 0.5) * (x + 0.5) + y * y + (z - 0.5) * (z - 0.5));
        double edge8 = std::sqrt((x + 0.5) * (x + 0.5) + y * y + (z + 0.5) * (z + 0.5));
        double edge9 = std::sqrt(x * x + (y - 0.5) * (y - 0.5) + (z - 0.5) * (z - 0.5));
        double edge10 = std::sqrt(x * x + (y - 0.5) * (y - 0.5) + (z + 0.5) * (z + 0.5));
        double edge11 = std::sqrt(x * x + (y + 0.5) * (y + 0.5) + (z - 0.5) * (z - 0.5));
        double edge12 = std::sqrt(x * x + (y + 0.5) * (y + 0.5) + (z + 0.5) * (z + 0.5));

        // If at least one of the distance is less than the radius (but not all), then the cell is intersecting the
        // sphere
        auto distances = { vertex1,
          vertex2,
          vertex3,
          vertex4,
          vertex5,
          vertex6,
          vertex7,
          vertex8,
          face1,
          face2,
          face3,
          face4,
          face5,
          face6,
          edge1,
          edge2,
          edge3,
          edge4,
          edge5,
          edge6,
          edge7,
          edge8,
          edge9,
          edge10,
          edge11,
          edge12 };

        int count = 0;
        for (auto distance : distances) {
          if (distance < radius) { count++; }
        }

        if (count > 0 && count < 8 + 6 + 12) { boundaryCells.push_back({ x, y, z }); }
      }
    }
  }

  return boundaryCells;
}

std::vector<std::array<int, 3>> getInnerCells(double radius)
{
  if (radius <= std::sqrt(0.75)) { return {}; }

  int cellRadius = static_cast<int>(radius) + 1;

  // Scan through the cells
  std::vector<std::array<int, 3>> innerCells{};
  for (int x = -cellRadius; x <= cellRadius; x++) {
    for (int y = -cellRadius; y <= cellRadius; y++) {
      for (int z = -cellRadius; z <= cellRadius; z++) {
        // Check if the cell is intersecting the sphere
        // Check distance with all vertices
        double vertex1 = std::sqrt((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) + (z - 0.5) * (z - 0.5));
        double vertex2 = std::sqrt((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) + (z + 0.5) * (z + 0.5));
        double vertex3 = std::sqrt((x - 0.5) * (x - 0.5) + (y + 0.5) * (y + 0.5) + (z - 0.5) * (z - 0.5));
        double vertex4 = std::sqrt((x - 0.5) * (x - 0.5) + (y + 0.5) * (y + 0.5) + (z + 0.5) * (z + 0.5));
        double vertex5 = std::sqrt((x + 0.5) * (x + 0.5) + (y - 0.5) * (y - 0.5) + (z - 0.5) * (z - 0.5));
        double vertex6 = std::sqrt((x + 0.5) * (x + 0.5) + (y - 0.5) * (y - 0.5) + (z + 0.5) * (z + 0.5));
        double vertex7 = std::sqrt((x + 0.5) * (x + 0.5) + (y + 0.5) * (y + 0.5) + (z - 0.5) * (z - 0.5));
        double vertex8 = std::sqrt((x + 0.5) * (x + 0.5) + (y + 0.5) * (y + 0.5) + (z + 0.5) * (z + 0.5));

        // If at least one of the distance is less than the radius (but not all), then the cell is intersecting the
        // sphere
        auto distances = { vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7, vertex8 };
        int count = 0;
        for (auto distance : distances) {
          if (distance < radius) { count++; }
        }

        // If all vertices are inside the sphere, then the cell is inside the sphere
        if (count == static_cast<int>(distances.size())) { innerCells.push_back({ x, y, z }); }
      }
    }
  }

  return innerCells;
}

std::vector<std::array<int, 3>> getBoundaryBetween(double radius1, double radius2)
{
  auto boundary_1 = getBoundaryCells(radius1);
  auto boundary_2 = getBoundaryCells(radius2);

  // Concatenate the two vectors
  std::vector<std::array<int, 3>> boundaryBetween{};
  boundaryBetween.insert(boundaryBetween.end(), boundary_1.begin(), boundary_1.end());
  boundaryBetween.insert(boundaryBetween.end(), boundary_2.begin(), boundary_2.end());

  // Remove duplicates
  std::sort(boundaryBetween.begin(), boundaryBetween.end());
  boundaryBetween.erase(std::unique(boundaryBetween.begin(), boundaryBetween.end()), boundaryBetween.end());

  return boundaryBetween;
}

std::vector<std::array<int, 3>> getInnerBetween(double radius1, double radius2)
{
  // Swap so that radius1 < radius2
  if (radius1 > radius2) { std::swap(radius1, radius2); }

  auto inner_1 = getInnerCells(radius1);
  auto inner_boundary = getBoundaryCells(radius1);
  auto inner_2 = getInnerCells(radius2);

  // Remove the cells in inner1 and inner_boundary that are in inner2
  for (auto cell : inner_1) {
    auto iter = std::find(inner_2.begin(), inner_2.end(), cell);
    if (iter != inner_2.end()) { inner_2.erase(iter); }
  }

  for (auto cell : inner_boundary) {
    auto iter = std::find(inner_2.begin(), inner_2.end(), cell);
    if (iter != inner_2.end()) { inner_2.erase(iter); }
  }

  return inner_2;
}