#pragma once
#include "../FindPath.h"
#include "../visualizationManager/visualization.h"
#include "../definitions.h"

class Vehicle {
public:
    virtual ~Vehicle() {}

    // Tüm araçlarýn implement etmesi gereken fonksiyon
    virtual FindPath::PathResult findPath(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,
        double speed) = 0;
    //arac adi donecke fonksýyona "kara " , "deniz","hava"    
    virtual QString name() const = 0;
};
