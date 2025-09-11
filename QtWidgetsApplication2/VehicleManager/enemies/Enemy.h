#pragma once

#include <cstdlib>
#include <ctime>
#include <QThread>
#include <QCoreApplication>
#include "./land/land.h"

class Enemy : public LandVehicle {
public:
    FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,
        double speed) override;
};
