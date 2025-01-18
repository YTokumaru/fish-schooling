#include "coordinate.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

Vect3 operator+(const Vect3 &lhs, const Vect3 &rhs)
{
  return { .x = lhs.x + rhs.x, .y = lhs.y + rhs.y, .z = lhs.z + rhs.z };
}

Vect3 operator-(const Vect3 &lhs, const Vect3 &rhs)
{
  return { .x = lhs.x - rhs.x, .y = lhs.y - rhs.y, .z = lhs.z - rhs.z };
}

Vect3 operator*(double scalar, const Vect3 &rhs)
{
  return { .x = scalar * rhs.x, .y = scalar * rhs.y, .z = scalar * rhs.z };
}

Vect3 operator*(const Vect3 &lhs, double scalar)
{
  return { .x = scalar * lhs.x, .y = scalar * lhs.y, .z = scalar * lhs.z };
}

Vect3 operator/(const Vect3 &lhs, double scalar)
{
  return { .x = lhs.x / scalar, .y = lhs.y / scalar, .z = lhs.z / scalar };
}

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

Vect3 periodic(const Vect3 &vect, unsigned int len)
{
  auto len_f = static_cast<double>(len);
  double x = vect.x - len_f;
  if (x < 0) { x += len_f; }

  double y = vect.y - len_f;
  if (y < 0) { y += len_f; }

  double z = vect.z - len_f;
  if (z < 0) { z += len_f; }

  return { .x = x, .y = y, .z = z };
}

double absolute(const Vect3 &vect) { return std::sqrt((vect.x * vect.x) + (vect.y * vect.y) + (vect.z * vect.z)); }

Vect3 normalize(const Vect3 &vect)
{
  const double magnitude = absolute(vect);
  return { .x = vect.x / magnitude, .y = vect.y / magnitude, .z = vect.z / magnitude };
}

Vect3 vect12(const Vect3 &vect1, const Vect3 &vect2)
{
  return { .x = vect2.x - vect1.x, .y = vect2.y - vect1.y, .z = vect2.z - vect1.z };
}

Vect3 vect12(const Vect3 &vect1, Vect3 vect2, unsigned int len)
{
  const Vect3 temp_vect = vect12(vect1, vect2);

  if (temp_vect.x > static_cast<double>(len) / 2) {
    vect2.x -= len;
  } else if (temp_vect.x < -static_cast<double>(len) / 2) {
    vect2.x += len;
  }
  if (temp_vect.y > static_cast<double>(len) / 2) {
    vect2.y -= len;
  } else if (temp_vect.y < -static_cast<double>(len) / 2) {
    vect2.y += len;
  }
  if (temp_vect.z > static_cast<double>(len) / 2) {
    vect2.z -= len;
  } else if (temp_vect.z < -static_cast<double>(len) / 2) {
    vect2.z += len;
  }

  return vect12(vect1, vect2);
}

unsigned int countInside(const std::array<int, 3> &cell, double radius, const Vect3 &center, bool count_boundary)
{

  constexpr std::array<Vect3, 6> faces_relpos = { { { .x = 0.5, .y = 0, .z = 0 },
    { .x = -0.5, .y = 0, .z = 0 },
    { .x = 0, .y = 0.5, .z = 0 },
    { .x = 0, .y = -0.5, .z = 0 },
    { .x = 0, .y = 0, .z = 0.5 },
    { .x = 0, .y = 0, .z = -0.5 } } };

  constexpr std::array<Vect3, 8> vertices_relpos = { { { .x = 0.5, .y = 0.5, .z = 0.5 },
    { .x = 0.5, .y = 0.5, .z = -0.5 },
    { .x = 0.5, .y = -0.5, .z = 0.5 },
    { .x = 0.5, .y = -0.5, .z = -0.5 },
    { .x = -0.5, .y = 0.5, .z = 0.5 },
    { .x = -0.5, .y = 0.5, .z = -0.5 },
    { .x = -0.5, .y = -0.5, .z = 0.5 },
    { .x = -0.5, .y = -0.5, .z = -0.5 } } };

  constexpr std::array<Vect3, 12> edges_relpos = { { { .x = 0.5, .y = 0.5, .z = 0 },
    { .x = 0.5, .y = -0.5, .z = 0 },
    { .x = 0.5, .y = 0, .z = 0.5 },
    { .x = 0.5, .y = 0, .z = -0.5 },
    { .x = 0, .y = 0.5, .z = 0.5 },
    { .x = 0, .y = 0.5, .z = -0.5 },
    { .x = -0.5, .y = 0.5, .z = 0 },
    { .x = -0.5, .y = -0.5, .z = 0 },
    { .x = -0.5, .y = 0, .z = 0.5 },
    { .x = -0.5, .y = 0, .z = -0.5 },
    { .x = 0, .y = -0.5, .z = 0.5 },
    { .x = 0, .y = -0.5, .z = -0.5 } } };

  // Join all the relative positions
  std::vector<Vect3> all_relpos{};
  all_relpos.insert(all_relpos.end(), faces_relpos.begin(), faces_relpos.end());
  all_relpos.insert(all_relpos.end(), vertices_relpos.begin(), vertices_relpos.end());
  all_relpos.insert(all_relpos.end(), edges_relpos.begin(), edges_relpos.end());

  unsigned int inside_count = 0;

  for (const auto &relpos : all_relpos) {
    const Vect3 pos = { .x = static_cast<double>(cell[0]) + relpos.x,
      .y = static_cast<double>(cell[1]) + relpos.y,
      .z = static_cast<double>(cell[2]) + relpos.z };
    const Vect3 vect = vect12(center, pos);
    if (count_boundary) {
      if (absolute(vect) < radius) { inside_count++; }
    } else {
      if (absolute(vect) <= radius) { inside_count++; }
    }
  }

  return inside_count;
}

bool isCellOnBoundary(const std::array<int, 3> &cell, double radius, const Vect3 &center)
{
  const unsigned int inside_count = countInside(cell, radius, center, true);

  const unsigned int face_count = 6;
  const unsigned int vertex_count = 8;
  const unsigned int edge_count = 12;

  return inside_count > 0 && inside_count < face_count + vertex_count + edge_count;
}

bool isCellInside(const std::array<int, 3> &cell, double radius, const Vect3 &center)
{
  const unsigned int inside_count = countInside(cell, radius, center, false);

  const unsigned int face_count = 6;
  const unsigned int vertex_count = 8;
  const unsigned int edge_count = 12;

  return inside_count == face_count + vertex_count + edge_count;
}

std::vector<std::array<int, 3>> getBoundaryCells(double radius, const Vect3 &center)
{
  std::vector<std::array<int, 3>> boundaryCells{};

  const int cell_radius = static_cast<int>(radius) + 1;

  for (int cell_x = -cell_radius; cell_x <= cell_radius; cell_x++) {
    for (int cell_y = -cell_radius; cell_y <= cell_radius; cell_y++) {
      for (int cell_z = -cell_radius; cell_z <= cell_radius; cell_z++) {
        if (isCellOnBoundary({ cell_x, cell_y, cell_z }, radius, center)) {
          boundaryCells.push_back({ cell_x, cell_y, cell_z });
        }
      }
    }
  }

  return boundaryCells;
}

std::vector<std::array<int, 3>> getInnerCells(double radius, const Vect3 &center)
{
  std::vector<std::array<int, 3>> innerCells{};


  const int cell_radius = static_cast<int>(radius) + 1;

  for (int cell_x = -cell_radius; cell_x <= cell_radius; cell_x++) {
    for (int cell_y = -cell_radius; cell_y <= cell_radius; cell_y++) {
      for (int cell_z = -cell_radius; cell_z <= cell_radius; cell_z++) {
        if (isCellInside({ cell_x, cell_y, cell_z }, radius, center)) {
          innerCells.push_back({ cell_x, cell_y, cell_z });
        }
      }
    }
  }

  return innerCells;
}


std::vector<std::array<int, 3>> getBoundaryCells(double radius)
{
  const std::array<Vect3, 6> faces_pos = { { { .x = 0.5, .y = 0, .z = 0 },
    { .x = -0.5, .y = 0, .z = 0 },
    { .x = 0, .y = 0.5, .z = 0 },
    { .x = 0, .y = -0.5, .z = 0 },
    { .x = 0, .y = 0, .z = 0.5 },
    { .x = 0, .y = 0, .z = -0.5 } } };

  const std::array<Vect3, 8> vertices_pos = { { { .x = 0.5, .y = 0.5, .z = 0.5 },
    { .x = 0.5, .y = 0.5, .z = -0.5 },
    { .x = 0.5, .y = -0.5, .z = 0.5 },
    { .x = 0.5, .y = -0.5, .z = -0.5 },
    { .x = -0.5, .y = 0.5, .z = 0.5 },
    { .x = -0.5, .y = 0.5, .z = -0.5 },
    { .x = -0.5, .y = -0.5, .z = 0.5 },
    { .x = -0.5, .y = -0.5, .z = -0.5 } } };

  const std::array<Vect3, 12> edges_pos = { { { .x = 0.5, .y = 0.5, .z = 0 },
    { .x = 0.5, .y = -0.5, .z = 0 },
    { .x = 0.5, .y = 0, .z = 0.5 },
    { .x = 0.5, .y = 0, .z = -0.5 },
    { .x = 0, .y = 0.5, .z = 0.5 },
    { .x = 0, .y = 0.5, .z = -0.5 },
    { .x = -0.5, .y = 0.5, .z = 0 },
    { .x = -0.5, .y = -0.5, .z = 0 },
    { .x = -0.5, .y = 0, .z = 0.5 },
    { .x = -0.5, .y = 0, .z = -0.5 },
    { .x = 0, .y = -0.5, .z = 0.5 },
    { .x = 0, .y = -0.5, .z = -0.5 } } };

  // Calculate the boundary cells with using the relative positions of the faces, vertices, and edges
  // and calculate the OR of the cells that are intersecting the sphere

  std::vector<std::array<int, 3>> boundary_cells{};

  for (const auto &face : faces_pos) {
    auto face_boundary_cells = getBoundaryCells(radius, face);
    boundary_cells.insert(boundary_cells.end(), face_boundary_cells.begin(), face_boundary_cells.end());
  }

  for (const auto &vertex : vertices_pos) {
    auto vertex_boundary_cells = getBoundaryCells(radius, vertex);
    boundary_cells.insert(boundary_cells.end(), vertex_boundary_cells.begin(), vertex_boundary_cells.end());
  }

  for (const auto &edge : edges_pos) {
    auto edge_boundary_cells = getBoundaryCells(radius, edge);
    boundary_cells.insert(boundary_cells.end(), edge_boundary_cells.begin(), edge_boundary_cells.end());
  }

  // Remove duplicates
  std::sort(boundary_cells.begin(), boundary_cells.end());
  boundary_cells.erase(std::unique(boundary_cells.begin(), boundary_cells.end()), boundary_cells.end());

  return boundary_cells;
}


std::vector<std::array<int, 3>> getInnerCells(double radius)
{
  const std::array<Vect3, 6> faces_pos = { { { .x = 0.5, .y = 0, .z = 0 },
    { .x = -0.5, .y = 0, .z = 0 },
    { .x = 0, .y = 0.5, .z = 0 },
    { .x = 0, .y = -0.5, .z = 0 },
    { .x = 0, .y = 0, .z = 0.5 },
    { .x = 0, .y = 0, .z = -0.5 } } };

  const std::array<Vect3, 8> vertices_pos = { { { .x = 0.5, .y = 0.5, .z = 0.5 },
    { .x = 0.5, .y = 0.5, .z = -0.5 },
    { .x = 0.5, .y = -0.5, .z = 0.5 },
    { .x = 0.5, .y = -0.5, .z = -0.5 },
    { .x = -0.5, .y = 0.5, .z = 0.5 },
    { .x = -0.5, .y = 0.5, .z = -0.5 },
    { .x = -0.5, .y = -0.5, .z = 0.5 },
    { .x = -0.5, .y = -0.5, .z = -0.5 } } };

  const std::array<Vect3, 12> edges_pos = { { { .x = 0.5, .y = 0.5, .z = 0 },
    { .x = 0.5, .y = -0.5, .z = 0 },
    { .x = 0.5, .y = 0, .z = 0.5 },
    { .x = 0.5, .y = 0, .z = -0.5 },
    { .x = 0, .y = 0.5, .z = 0.5 },
    { .x = 0, .y = 0.5, .z = -0.5 },
    { .x = -0.5, .y = 0.5, .z = 0 },
    { .x = -0.5, .y = -0.5, .z = 0 },
    { .x = -0.5, .y = 0, .z = 0.5 },
    { .x = -0.5, .y = 0, .z = -0.5 },
    { .x = 0, .y = -0.5, .z = 0.5 },
    { .x = 0, .y = -0.5, .z = -0.5 } } };

  // Calculate the inner cells with using the relative positions of the faces, vertices, and edges
  // and calculate the AND of the cells that are inside the sphere

  std::vector<std::array<int, 3>> inner_cells{};

  for (const auto &face : faces_pos) {
    auto face_inner_cells = getInnerCells(radius, face);
    inner_cells.insert(inner_cells.end(), face_inner_cells.begin(), face_inner_cells.end());
  }

  for (const auto &vertex : vertices_pos) {
    auto vertex_inner_cells = getInnerCells(radius, vertex);
    inner_cells.insert(inner_cells.end(), vertex_inner_cells.begin(), vertex_inner_cells.end());
  }

  for (const auto &edge : edges_pos) {
    auto edge_inner_cells = getInnerCells(radius, edge);
    inner_cells.insert(inner_cells.end(), edge_inner_cells.begin(), edge_inner_cells.end());
  }

  // Only keep the duplicates
  std::vector<std::array<int, 3>> inner_cells_and{};

  const int face_count = 6;
  const int vertex_count = 8;
  const int edge_count = 12;

  for (const auto &cell : inner_cells) {
    if (std::count(inner_cells.begin(), inner_cells.end(), cell) == face_count + vertex_count + edge_count) {
      inner_cells_and.push_back(cell);
    }
  }

  // Remove duplicates
  std::sort(inner_cells_and.begin(), inner_cells_and.end());
  inner_cells_and.erase(std::unique(inner_cells_and.begin(), inner_cells_and.end()), inner_cells_and.end());

  return inner_cells_and;
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
