#ifndef FIND_PATH_H
#define FIND_PATH_H

#include <vector>
#include <cmath>
#include "definitions.h"   // k���k harf ile

class FindPath {
public:
    enum class Vehicle { Land, Sea, Air };

    struct Cell {
        int r, c;
        bool operator==(const Cell& other) const {
            return r == other.r && c == other.c;
        }
    };

    using Grid = std::vector<std::vector<int>>;

    struct PathResult {
        std::vector<Cell> nodes;
        double cost = 0.0;  // Land/Sea i�in
        double distance = 0.0;  // Air i�in
        double time = 0.0;  // Air i�in
    };

    // Ortak yard�mc�lar
    static bool inBounds(int r, int c, int R, int C);
    static int manhattan(Cell a, Cell b);

    // Ana yol bulucu
    static PathResult findPath(const Grid& grid, Cell start, Cell goal, Vehicle vehicle);

    // Air �zel
    static PathResult findAirPath(Cell start, Cell goal, const Speed& speed);
};

#endif // FIND_PATH_H
