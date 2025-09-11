#include "threadmanager.h"
#include <QtConcurrent>
#include "visualization.h"

ThreadManager::ThreadManager(QObject* parent) : QObject(parent) {}

void ThreadManager::runVehicle(Vehicle* vehicle,
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    QTableWidget* table,double speed)
{
    QtConcurrent::run([=]() {
        Visualization viz(table);

        double speed = 1.0;
        if (vehicle->name() == "Kara") speed = Speed::land;
        else if (vehicle->name() == "Deniz") speed = Speed::sea;
        else if (vehicle->name() == "Hava") speed = Speed::air;

        auto res = vehicle->findPath(grid, start, goal, &viz, speed);
        emit vehicleFinished(vehicle->name(), res);
        });
}
