#pragma once
#include "vehicle/vehicle.h"
#include "EnemyManager/EnemyManager.h"

class Enemy : public Vehicle {
public:
    Enemy();

    // Vehicle'dan override edilen fonksiyonlar
    FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,     // düşmanın sabit hedefi yok  random kullanılacak
        Visualization* viz,
        double speed,
        EnemyManager* enemies) override;

    QString name() const override { return "Düşman"; }
};
