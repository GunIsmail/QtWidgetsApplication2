#ifndef SEA_H
#define SEA_H

#include "FindPath.h"
#include "visualization.h"

class Sea {
public:
    static FindPath::PathResult parallelSearch(
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        Visualization* viz,double speed);   
};

#endif // SEA_H
