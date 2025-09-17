#pragma once
#include "vehicle/vehicle.h"

class AirVehicle : public Vehicle {
public:
    FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,
        double speed,
        EnemyManager*) override;

    QString name() const override 
    {
        return "Hava";
    }
};
