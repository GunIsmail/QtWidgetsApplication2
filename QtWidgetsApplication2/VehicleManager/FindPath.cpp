#include "findpath.h"
#include "land/land.h"
#include "sea/sea.h"

bool FindPath::inBounds(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

int FindPath::manhattan(Cell a, Cell b) {
    return std::abs(a.r - b.r) + std::abs(a.c - b.c);
}

FindPath::PathResult FindPath::findPath(const Grid& grid, Cell start, Cell goal, Vehicle vehicle) {
    switch (vehicle) {
    case Vehicle::Land:
        return Land::aStar(grid, start, goal);

    case Vehicle::Sea:
        //return Sea::findPath(grid, start, goal);

    case Vehicle::Air: {
        Speed defaultSpeed;
        return findAirPath(start, goal, defaultSpeed);
    }

    default:
        return {};
    }
}

FindPath::PathResult FindPath::findAirPath(Cell start, Cell goal, const Speed& speed) {
    PathResult result;

    double dx = static_cast<double>(goal.r - start.r);
    double dy = static_cast<double>(goal.c - start.c);

    result.distance = std::sqrt(dx * dx + dy * dy);
    result.time = (speed.air > 0) ? result.distance / speed.air : -1.0;

    return result;
}
