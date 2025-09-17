#pragma once
#include "definitions.h"
#include <vector>
#include <FindPath.h>
#include <QtGlobal> // qint64 i�in

struct EnemyUnit {
    FindPath::Cell pos;   // mevcut konum
    double speed;         // h�cre/sn
    qint64 lastMoveTime;  // son hareket zaman� (ms)
};

class EnemyManager {
public:
    EnemyManager(const FindPath::Grid& grid);

    void addEnemy(const FindPath::Cell& start, double speed = 1.0);
    void stepAll(); // h�zlar�na g�re d��manlar� g�nceller

    // snapshot�� g�ncelle (her 10 ms timer buray� �a��racak)
    void updateSnapshot(int R, int C);

    // d��ar�ya bilgi sa�layan fonksiyonlar
    const std::vector<EnemyUnit>& enemies() const;
    bool isOccupied(int r, int c) const;

    const FindPath::Grid& grid() const;
    const FindPath::Grid& currentEnemyMatrix() const { return m_enemySnapshot; }

    // anl�k d��man matrisini olu�tur (tek seferlik kullan�m i�in)
    FindPath::Grid getEnemyMatrix(int R, int C) const;

private:
    FindPath::Grid m_grid;            // sabit engel matrisi (kara/deniz vs.)
    std::vector<EnemyUnit> m_enemies; // aktif d��man listesi
    FindPath::Grid m_enemySnapshot;   // her 10 ms�de g�ncellenen matris
};
