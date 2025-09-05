#include "air.h"
#include <cmath>

FindPath::PathResult Air::findPath(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal) {
    FindPath::PathResult out;

    float dx = static_cast<float>(goal.c - start.c);
    float dy = static_cast<float>(goal.r - start.r);
    float distance = std::sqrt(dx * dx + dy * dy);

    out.distance = distance;

    
    Speed vehicleSpeeds;
    float airSpeed = vehicleSpeeds.air;
    float timeInSeconds = distance / airSpeed;

    
    out.nodes.push_back(start);
    out.nodes.push_back(goal);
   


    return out;
}