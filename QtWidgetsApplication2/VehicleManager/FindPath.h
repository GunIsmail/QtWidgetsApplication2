#ifndef FINDPATH_H
#define FINDPATH_H

#include <vector>
#include <cmath>
#include <QString>

class Visualization;  // ileri bildirim
class Vehicle;        // abstract vehicle sınıfı

namespace FindPath {

    struct Cell {
        int r, c;
        bool operator==(const Cell& other) const {
            return r == other.r && c == other.c;
        }
    };

    struct PathResult {
        std::vector<Cell> nodes;
        std::vector<Cell> mines;  // mayınlı hücreler
        double distance = 0.0;
    };

    using Grid = std::vector<std::vector<int>>;

    // Yardımcı fonksiyonlar
    bool inBounds(int r, int c, int R, int C);
    int manhattan(Cell a, Cell b);

    // Polimorfizm saglayan yapi
    PathResult findPathWithVehicle(
        Vehicle* vehicle,
        const Grid& grid,
        Cell start,
        Cell goal,
        Visualization* viz
    );
}

#endif // FINDPATH_H
