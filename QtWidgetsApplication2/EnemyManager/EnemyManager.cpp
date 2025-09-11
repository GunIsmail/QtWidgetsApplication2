#include "EnemyManager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

static bool passableForLand(const FindPath::Grid& grid, int r, int c) {
    if (!FindPath::inBounds(r, c, (int)grid.size(), (int)grid[0].size()))
        return false;
    return grid[r][c] == 0; // sadece kara hücre geçerli
}

EnemyManager::EnemyManager(const FindPath::Grid& grid)
    : m_grid(grid)
{
    std::srand((unsigned)std::time(nullptr)); 

}
const FindPath::Grid& EnemyManager::grid() const {
    return m_grid;
}


void EnemyManager::addEnemy(const FindPath::Cell& start) {
    m_enemies.push_back({ start });
}
// cagirdiginde tum dusman birimleri hareket etmeye baslatir . 
void EnemyManager::stepAll() {
    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    // önce mevcut pozisyonlarý iþaretle
    std::set<std::pair<int, int>> occupied;
    for (auto& e : m_enemies) {
        occupied.insert({ e.pos.r, e.pos.c });
    }

    // her düþmaný sýrayla hareket ettir
    for (auto& e : m_enemies) {
        std::vector<FindPath::Cell> neighbors;

        for (int k = 0; k < 4; k++) {
            int nr = e.pos.r + dr[k];
            int nc = e.pos.c + dc[k];
            if (passableForLand(m_grid, nr, nc) &&
                !occupied.count({ nr, nc })) {
                neighbors.push_back({ nr, nc });
            }
        }

        if (!neighbors.empty()) {
            // rastgele bir komþu seç
            auto next = neighbors[std::rand() % neighbors.size()];

            // occupied güncelle
            occupied.erase({ e.pos.r, e.pos.c });
            e.pos = next;
            occupied.insert({ e.pos.r, e.pos.c });
        }
    }
}

const std::vector<EnemyUnit>& EnemyManager::enemies() const { // dusman vectoru doner 
    return m_enemies;
}

// o gridde dusman var mý yok mu kontreol eder 
bool EnemyManager::isOccupied(int r, int c) const {
    for (auto& e : m_enemies) {
        if (e.pos.r == r && e.pos.c == c)
            return true;
    }
    return false;
}
