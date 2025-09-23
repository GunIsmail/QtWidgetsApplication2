#include "findpath.h"
#include "vehicle/vehicle.h"
#include "definitions.h"
#include "EnemyManager/EnemyManager.h"

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
    Visualization* viz,
    EnemyManager* enemies)   // parametre burada da var
{
    if (!vehicle) return {};

    // h�z tan�mlar� definitions.h i�inden al�n�r
    double speed = 0;
    if (vehicle->name() == "Kara") {
        speed = Speed::land;
    }
    else if (vehicle->name() == "Deniz") {
        speed = Speed::sea;
    }
    else if (vehicle->name() == "Hava") {
        speed = Speed::air;
    }

    //  B�t�n ara�lar ayn� imzay� kullan�r
    return vehicle->findPath(grid, start, goal, viz, speed, enemies);
}
