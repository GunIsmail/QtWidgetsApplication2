#include "Enemy.h"


static bool passableForLand(const FindPath::Grid& grid, int r, int c) {
    if (!FindPath::inBounds(r, c, (int)grid.size(), (int)grid[0].size()))
        return false;
    return grid[r][c] == 0; // 0 = Kara
}

FindPath::PathResult Enemy::findPath(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell /*goal*/,   // düþmanýn hedefi yok  random
    Visualization* viz,
    double speed)
{
    FindPath::PathResult out;
    FindPath::Cell cur = start;
    out.nodes.push_back(cur);

    std::srand((unsigned)std::time(nullptr));

    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    for (int steps = 0; steps <10000 ; steps++) { 
        std::vector<FindPath::Cell> neighbors;
        for (int k = 0; k < 4; k++) {
            int nr = cur.r + dr[k];
            int nc = cur.c + dc[k];
            if (passableForLand(grid, nr, nc)) {
                neighbors.push_back({ nr, nc });
            }
        }

        if (neighbors.empty()) break; // çýkmaz sokak

        // rastgele komþu seç
        cur = neighbors[std::rand() % neighbors.size()];
        out.nodes.push_back(cur);

        if (viz) {
            auto table = viz->table();
            if (table) {
                QTableWidgetItem* item = table->item(cur.r, cur.c);
                if (item) {
                    item->setIcon(VisualizationConfig::landIcon()); // tank ikonu
                }
                QCoreApplication::processEvents();
                QThread::msleep(std::max(10, (int)(VisualizationConfig::STEP_DELAY_MS / speed)));
            }
        }
    }

    return out;
}