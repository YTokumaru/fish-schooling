#include "coordinate.hpp"

std::vector<std::array<int, 3>> getBoundaryCells(double radius)
{

    if (radius <= 0)
    {
        return {};
    }

    if (radius <= 0.5)
    {
        return {{0, 0, 0}};
    }


    int cellRadius = static_cast<int>(radius) + 1;

    // Scan through the cells
    std::vector<std::array<int, 3>> boundaryCells{};
    for (int x = -cellRadius; x <= cellRadius; x++)
    {
        for (int y = -cellRadius; y <= cellRadius; y++)
        {
            for (int z = -cellRadius; z <= cellRadius; z++)
            {
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

                // If at least one of the distance is less than the radius (but not all), then the cell is intersecting the sphere
                auto distances = {vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7, vertex8,
                                    face1, face2, face3, face4, face5, face6,
                                    edge1, edge2, edge3, edge4, edge5, edge6, edge7, edge8, edge9, edge10, edge11, edge12};

                int count = 0;
                for (auto distance : distances)
                {
                    if (distance < radius)
                    {
                        count++;
                    }
                }

                if (count > 0 && count < 8 + 6 + 12)
                {
                    boundaryCells.push_back({x, y, z});
                }
            }
        }
    }

    return boundaryCells;
}

std::vector<std::array<int, 3>> getInnerCells(double radius)
{
    if (radius <= std::sqrt(0.75))
    {
        return {};
    }

    int cellRadius = static_cast<int>(radius) + 1;

    // Scan through the cells
    std::vector<std::array<int, 3>> innerCells{};
    for (int x = -cellRadius; x <= cellRadius; x++)
    {
        for (int y = -cellRadius; y <= cellRadius; y++)
        {
            for (int z = -cellRadius; z <= cellRadius; z++)
            {
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

                // If at least one of the distance is less than the radius (but not all), then the cell is intersecting the sphere
                auto distances = {vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7, vertex8};
                int count = 0;
                for (auto distance : distances)
                {
                    if (distance < radius)
                    {
                        count++;
                    }
                }

                // If all vertices are inside the sphere, then the cell is inside the sphere
                if (count == distances.size())
                {
                    innerCells.push_back({x, y, z});
                }
            }
        }
    }

    return innerCells;
}

std::vector<std::array<int, 3>> getBoundaryBetween(double radius_1, double radius_2)
{
    auto boundary_1 = getBoundaryCells(radius_1);
    auto boundary_2 = getBoundaryCells(radius_2);

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
    if (radius1 > radius2)
    {
        std::swap(radius1, radius2);
    }

    auto inner_1 = getInnerCells(radius1);
    auto inner_boundary = getBoundaryCells(radius1);
    auto inner_2 = getInnerCells(radius2);

    // Remove the cells in inner1 and inner_boundary that are in inner2
    for(auto cell: inner_1)
    {
        auto it = std::find(inner_2.begin(), inner_2.end(), cell);
        if(it != inner_2.end())
        {
            inner_2.erase(it);
        }
    }
    
    for (auto cell : inner_boundary)
    {
        auto it = std::find(inner_2.begin(), inner_2.end(), cell);
        if (it != inner_2.end())
        {
            inner_2.erase(it);
        }
    }

    return inner_2;
}