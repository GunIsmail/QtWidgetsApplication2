#ifndef FINDPATH_H
#define FINDPATH_H

#include <vector>
#include <cmath>
#include <QString>

class Visualization;   // forward declaratipn 
class Vehicle;         // abstract vehicle sınıfı
class EnemyManager;    //  düşmanları desteklemek için ileri bildirim

namespace FindPath {

    struct Cell {
        int r, c;
        bool operator==(const Cell& other) const {
            return r == other.r && c == other.c;
        }
        bool operator<(const Cell& other) const {
            if (r != other.r) return r < other.r;
            return c < other.c;
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

    // Polimorfizm saglayan yapı
    PathResult findPathWithVehicle(
        Vehicle* vehicle,
        const Grid& grid,
        Cell start,
        Cell goal,
        Visualization* viz,
        EnemyManager* enemies   // 🔹 buraya eklendi
    );
}

#endif // FINDPATH_H
