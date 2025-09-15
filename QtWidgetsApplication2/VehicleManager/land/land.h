#pragma once
#include "../vehicle/vehicle.h"
#include "../EnemyManager/EnemyManager.h  "
#include <QCoreApplication>
#include <qthread.h>

class LandVehicle : public Vehicle {
public:
    LandVehicle() : enemyManager(nullptr) {}
    ~LandVehicle() override {}

    void setEnemyManager(EnemyManager* mgr) {
        enemyManager = mgr;
}

    FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,
        double speed) override;

    QString name() const override { return "Kara Aracý"; }

private:
    EnemyManager* enemyManager; // sadece LandVehicle kullanacak
};
