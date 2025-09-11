#ifndef LAND_H
#define LAND_H
#include "vehicle/vehicle.h"
#include "FindPath.h"
#include "visualization.h"

class LandVehicle : public Vehicle {
public:
    FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,
        double speed) override;

    QString name() const override { return "Kara"; }
}; 
#endif // LAND_H
