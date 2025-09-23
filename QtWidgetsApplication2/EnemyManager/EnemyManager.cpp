#include "EnemyManager.h"
#include <cstdlib>
#include <ctime>
#include <map>
#include <set>
#include <QDateTime>

// sabit engel kontrolü (sadece kara hücre geçilebilir)
static bool passableForLand(const FindPath::Grid& grid,
    const EnemyManager* enemies,
    int r, int c) {
    if (!FindPath::inBounds(r, c, (int)grid.size(), (int)grid[0].size()))
        return false;
    if (grid[r][c] != 0) return false;

    if (enemies) {
        const auto& em = enemies->currentEnemyMatrix();
        if (r >= 0 && r < (int)em.size() &&
            c >= 0 && c < (int)em[0].size()) {
            if (em[r][c] == 1) return false; // düşman snapshot kontrolü
        }
    }
    return true;
}

EnemyManager::EnemyManager(const FindPath::Grid& grid)
    : m_grid(grid)
{
    std::srand(std::time(nullptr));
}

void EnemyManager::addEnemy(const FindPath::Cell& start, double speed) {
    EnemyUnit e{ start, speed, QDateTime::currentMSecsSinceEpoch() };

    m_enemies.push_back(e);
    qDebug() << "Yeni düşman eklendi -> grid:" << start.r << "," << start.c;

}

void EnemyManager::stepAll() {
    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    std::map<FindPath::Cell, FindPath::Cell> nextTargets;
    std::set<FindPath::Cell> occupied;

    for (auto& e : m_enemies) {
        // === HIZ KONTROLÜ ===
        if (e.speed <= 0) {
            // Hızı 0 veya negatif → hiç hareket etmiyor
            nextTargets[e.pos] = e.pos;
            continue;
        }

        double interval = VisualizationConfig::STEP_DELAY_MS / e.speed; // ms per step
        if (now - e.lastMoveTime < interval)
            continue; // zamanı gelmedi

        // Komşuları kontrol et
        std::vector<FindPath::Cell> neighbors;
        for (int k = 0; k < 4; k++) {
            int nr = e.pos.r + dr[k];
            int nc = e.pos.c + dc[k];
            FindPath::Cell neigh{ nr, nc };

            if (passableForLand(m_grid, this, nr, nc) &&
                occupied.find(neigh) == occupied.end() &&
                !isOccupied(nr, nc)) {
                neighbors.push_back(neigh);
            }
        }

        if (!neighbors.empty()) {
            auto next = neighbors[std::rand() % neighbors.size()];
            nextTargets[e.pos] = next;
            occupied.insert(next);
            e.lastMoveTime = now; // sadece hareket ettiyse güncelle
        }
        else {
            nextTargets[e.pos] = e.pos; // hareket edemedi
            // lastMoveTime aynı kalmalı
        }
    }


    // Sonuçları uygula
    for (auto& e : m_enemies) {
        if (nextTargets.count(e.pos)) {
            e.pos = nextTargets[e.pos];
        }
    }
}


void EnemyManager::updateSnapshot(int R, int C) {
    m_enemySnapshot.assign(R, std::vector<int>(C, 0));

    for (auto& e : m_enemies) {
        if (e.pos.r >= 0 && e.pos.r < R &&
            e.pos.c >= 0 && e.pos.c < C) {
            m_enemySnapshot[e.pos.r][e.pos.c] = 1;
        }
    }
}

const std::vector<EnemyUnit>& EnemyManager::enemies() const {
    return m_enemies;
}

bool EnemyManager::isOccupied(int r, int c) const {
    for (auto& e : m_enemies) {
        if (e.pos.r == r && e.pos.c == c)
            return true;
    }
    return false;
}

const FindPath::Grid& EnemyManager::grid() const {
    return m_grid;
}

FindPath::Grid EnemyManager::getEnemyMatrix(int R, int C) const {
    FindPath::Grid m(R, std::vector<int>(C, 0));
    for (auto& e : m_enemies) {
        if (e.pos.r >= 0 && e.pos.r < R && e.pos.c >= 0 && e.pos.c < C)
            m[e.pos.r][e.pos.c] = 1;
    }
    return m;
}
