#include "findpath.h"
#include "land/land.h"
#include "sea/sea.h"
#include "air/air.h"   
#include "definitions.h"

bool FindPath::inBounds(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

int FindPath::manhattan(Cell a, Cell b) {
    return std::abs(a.r - b.r) + std::abs(a.c - b.c);
}

FindPath::PathResult FindPath::findPath(const Grid& grid,
    Cell start,
    Cell goal,
    Vehicle vehicle,
    Visualization* viz)   
{
    switch (vehicle) {
    case Vehicle::Land:
        return Land::aStar(grid, start, goal, viz);

    case Vehicle::Sea:
        return Sea::parallelSearch(grid, start, goal, viz);

    case Vehicle::Air:
        return Air::findPath(grid, start, goal, viz);

    default:
        return {};
    }
}
