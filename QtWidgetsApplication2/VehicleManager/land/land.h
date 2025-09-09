#ifndef LAND_H
#define LAND_H

#include "FindPath.h"
#include "visualization.h"

class Land {
public:
    static FindPath::PathResult aStar(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz = nullptr  // gui boymaa �c�n
    );
};

#endif // LAND_H
