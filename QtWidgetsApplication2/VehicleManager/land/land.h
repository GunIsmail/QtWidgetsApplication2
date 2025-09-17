#pragma once
#include "vehicle/vehicle.h"
#include "EnemyManager/EnemyManager.h"
#include <vector>

class LandVehicle : public Vehicle {
public:
    LandVehicle();

    // Vehicle'dan override edilen fonksiyonlar
    FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,
        double speed,
        EnemyManager* enemies) override;

    bool stepMove(const FindPath::Grid& grid,
        EnemyManager* enemies,
        QTableWidget* table) override;   

    QString name() const override { return "Kara"; }

    FindPath::Cell currentPos() const { return m_currentPos; }
    void setCurrentPos(const FindPath::Cell& pos) { m_currentPos = pos; }

private:
    static bool passableForLand(const FindPath::Grid& grid,
        const EnemyManager* enemies,
        int r, int c);

    static std::vector<FindPath::Cell> runAStar(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        const EnemyManager* enemies);

    FindPath::Cell m_currentPos;
    FindPath::Cell m_goal;                       // 🔹 game loop için hedef
    std::vector<FindPath::Cell> m_path;          // 🔹 aktif yol
    int m_stepIndex = 0;                         // 🔹 path index
};
