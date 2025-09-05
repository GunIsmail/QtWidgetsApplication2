#ifndef LAND_H
#define LAND_H

#include "../FindPath.h" // Üst klasördeki FindPath.h'yi dahil et

namespace Land {
    FindPath::PathResult aStar(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal);
}

#endif // LAND_H