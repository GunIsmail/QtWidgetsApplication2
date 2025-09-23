    #pragma once
#include "../FindPath.h"
#include "../visualizationManager/visualization.h"
#include "definitions.h"

class EnemyManager; // pointer gecirebilmek �c�n forward declaration

class Vehicle {
public:
    virtual ~Vehicle() {}

    virtual FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,
        double speed,
        EnemyManager* enemies) = 0; // pure virtual fonksiyon yapt�k. Tan�mlamadan kullanamazs�n. 
   
    virtual QString name() const = 0; //arac adi donecke fonks�yona "kara " , "deniz","hava"    
    virtual bool stepMove(const FindPath::Grid& grid,
        EnemyManager* enemies,
        QTableWidget* table) {
        Q_UNUSED(grid);
        Q_UNUSED(enemies);
        Q_UNUSED(table);
        return false;
    }

};
