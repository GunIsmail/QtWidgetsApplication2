#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <QThread>
#include <QCoreApplication>
#include <QTableWidgetItem>
#include <QDebug>

static bool passableForLand(const FindPath::Grid& grid,
    const EnemyManager* enemies,
    int r, int c) {
    if (!FindPath::inBounds(r, c, (int)grid.size(), (int)grid[0].size()))
        return false;
    if (grid[r][c] != 0) return false; // sadece kara hücre geçerli

    if (enemies) {
        const auto& em = enemies->currentEnemyMatrix();
        if (r >= 0 && r < (int)em.size() &&
            c >= 0 && c < (int)em[0].size()) {
            if (em[r][c] == 1) return false; // başka düşman varsa geçme
        }
    }
    return true;
}

Enemy::Enemy() {
    // constructor şimdilik boş
}

FindPath::PathResult Enemy::findPath(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell /*goal*/,   // düşman için kullanılmıyor
    Visualization* viz,
    double speed,
    EnemyManager* enemies)
{
    FindPath::PathResult out;
    FindPath::Cell cur = start;
    out.nodes.push_back(cur);

    std::srand((unsigned)std::time(nullptr));

    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    for (int steps = 0; steps < 500; steps++) {
        std::vector<FindPath::Cell> neighbors;
        for (int k = 0; k < 4; k++) {
            int nr = cur.r + dr[k];
            int nc = cur.c + dc[k];
            if (passableForLand(grid, enemies, nr, nc)) {
                neighbors.push_back({ nr, nc });
            }
        }

        if (neighbors.empty()) break; // çıkmaz sokak

        // rastgele komşu seç
        cur = neighbors[std::rand() % neighbors.size()];
        out.nodes.push_back(cur);

        if (viz) {
            auto table = viz->table();
            if (table) {
                if (auto* item = table->item(cur.r, cur.c)) {
                    item->setIcon(VisualizationConfig::enemyIcon());
                }
                QCoreApplication::processEvents();
                QThread::msleep(std::max(10, (int)(VisualizationConfig::STEP_DELAY_MS / speed)));
            }
        }
    }

    return out;
}
