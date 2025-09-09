#ifndef AIR_H
#define AIR_H

#include "findpath.h"
#include "visualization.h"

class Air {
public:
    static FindPath::PathResult findPath(const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz);
};

#endif // AIR_H
