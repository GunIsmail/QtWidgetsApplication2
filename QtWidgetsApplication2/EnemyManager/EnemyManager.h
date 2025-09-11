#pragma once
#include "definitions.h"
#include <vector>
#include <set>
#include "FindPath.h"

struct EnemyUnit

{
    FindPath::Cell pos;  // düþmanýn bulunduðu hücre
};


class EnemyManager {
public:
    EnemyManager(const FindPath::Grid& grid);
    const FindPath::Grid& grid() const;

    void addEnemy(const FindPath::Cell& start);  
    void stepAll();                              
    const std::vector<EnemyUnit>& enemies() const; 

    bool isOccupied(int r, int c) const;         

private:
    const FindPath::Grid& m_grid;
    std::vector<EnemyUnit> m_enemies;
};

