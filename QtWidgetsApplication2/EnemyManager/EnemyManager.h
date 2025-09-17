#pragma once
#include "definitions.h"
#include <vector>
#include <FindPath.h>
#include <QtGlobal> // qint64 için

struct EnemyUnit {
    FindPath::Cell pos;   // mevcut konum
    double speed;         // hücre/sn
    qint64 lastMoveTime;  // son hareket zamaný (ms)
};

class EnemyManager {
public:
    EnemyManager(const FindPath::Grid& grid);

    void addEnemy(const FindPath::Cell& start, double speed = 1.0);
    void stepAll(); // hýzlarýna göre düþmanlarý günceller

    // snapshot’ý güncelle (her 10 ms timer burayý çaðýracak)
    void updateSnapshot(int R, int C);

    // dýþarýya bilgi saðlayan fonksiyonlar
    const std::vector<EnemyUnit>& enemies() const;
    bool isOccupied(int r, int c) const;

    const FindPath::Grid& grid() const;
    const FindPath::Grid& currentEnemyMatrix() const { return m_enemySnapshot; }

    // anlýk düþman matrisini oluþtur (tek seferlik kullaným için)
    FindPath::Grid getEnemyMatrix(int R, int C) const;

private:
    FindPath::Grid m_grid;            // sabit engel matrisi (kara/deniz vs.)
    std::vector<EnemyUnit> m_enemies; // aktif düþman listesi
    FindPath::Grid m_enemySnapshot;   // her 10 ms’de güncellenen matris
};
