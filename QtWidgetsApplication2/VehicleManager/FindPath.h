#ifndef FINDPATH_H
#define FINDPATH_H

#include <vector>
#include <cmath>
#include <QString>

class Visualization;  // ileri bildirim
class Vehicle;        // abstract vehicle s�n�f�

namespace FindPath {

    struct Cell {
        int r, c;
        bool operator==(const Cell& other) const {
            return r == other.r && c == other.c;
        }
    };

    struct PathResult {
        std::vector<Cell> nodes;
        std::vector<Cell> mines;  // may�nl� h�creler
        double distance = 0.0;
    };

    using Grid = std::vector<std::vector<int>>;

    // Yard�mc� fonksiyonlar
    bool inBounds(int r, int c, int R, int C);
    int manhattan(Cell a, Cell b);

    // Polimorfik yol bulma
    PathResult findPathWithVehicle(
        Vehicle* vehicle,
        const Grid& grid,
        Cell start,
        Cell goal,
        Visualization* viz
    );
}

#endif // FINDPATH_H
