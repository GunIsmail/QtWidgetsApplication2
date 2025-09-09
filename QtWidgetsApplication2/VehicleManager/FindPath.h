#ifndef FIND_PATH_H
#define FIND_PATH_H

#include <vector>
#include <cmath>
#include "definitions.h"
#include <QDebug>

class Visualization;

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
        std::vector<Cell> mines;
        double cost = 0.0;     // Land / Sea için
        double distance = 0.0; // Air için
        double time = 0.0;     // Air için
    };

    // Ortak yardýmcýlar
    static bool inBounds(int r, int c, int R, int C);
    static int manhattan(Cell a, Cell b);

    // Ana yol bulucu
    static PathResult findPath(const Grid& grid,
        Cell start,
        Cell goal,
        Vehicle vehicle,
        Visualization* viz);   

    // Air özel
    static PathResult findAirPath(Cell start, Cell goal, const Speed& speed);
};

#endif // FIND_PATH_H
