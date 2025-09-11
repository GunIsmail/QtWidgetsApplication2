#include "findpath.h"
#include "vehicle/vehicle.h"
#include "definitions.h"

bool FindPath::inBounds(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

int FindPath::manhattan(Cell a, Cell b) {
    return std::abs(a.r - b.r) + std::abs(a.c - b.c);
}

FindPath::PathResult FindPath::findPathWithVehicle(
    Vehicle* vehicle,
    const Grid& grid,
    Cell start,
    Cell goal,
    Visualization* viz)
{
    if (!vehicle) return {};

    // hýz tanýmlarý definitions.h içinden alýnýr
    double speed = 1.0;
    if (vehicle->name() == "Kara") {
        speed = Speed::land;
    }
    else if (vehicle->name() == "Deniz") {
        speed = Speed::sea;
    }
    else if (vehicle->name() == "Hava") {
        speed = Speed::air;
    }

    return vehicle->findPath(grid, start, goal, viz, speed);
}
