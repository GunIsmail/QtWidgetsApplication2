#include "land.h"
#include <queue>
#include <map>
#include <set>
#include <limits>
#include <cmath>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include <QColor>


//// @ Her adımda A* çalışmıyor.
//
//Araç normalde önceden bulduğu yolu kullanıyor.
// Dinamik A* 



// Komşu hareket yönleri
static const int dr[4] = { -1, 1, 0, 0 };
static const int dc[4] = { 0, 0, -1, 1 };

using Cell = FindPath::Cell;

LandVehicle::LandVehicle() {
    m_currentPos = { 0, 0 };
    m_stepIndex = 0;
    m_path.clear();
    m_goal = { -1, -1 };
}

// Hücre kara + düşman snapshot kontrolü
bool LandVehicle::passableForLand(const FindPath::Grid& grid,
    const EnemyManager* enemies,
    int r, int c)
{
    int safeRadius = 1;
    if (!FindPath::inBounds(r, c, (int)grid.size(), (int)grid[0].size()))
        return false;
    if (grid[r][c] != 0)
        return false;

    if (enemies) {
        const auto& em = enemies->currentEnemyMatrix();
        if (r >= 0 && r < (int)em.size() &&
            c >= 0 && c < (int)em[0].size()) {
            if (em[r][c] == 1) return false;

            // güvenlik yarıçapı
            for (int rr = -safeRadius; rr <= safeRadius; rr++) {
                for (int cc = -safeRadius; cc <= safeRadius; cc++) {
                    int nr = r + rr, nc = c + cc;
                    if (FindPath::inBounds(nr, nc, (int)em.size(), (int)em[0].size())) {
                        if (em[nr][nc] == 1) return false;
                    }
                }
            }
        }
    }
    return true;
}

// 🔹 Tek adım ilerleme (game loop için)
bool LandVehicle::stepMove(const FindPath::Grid& grid,
    EnemyManager* enemies,
    QTableWidget* table)
{
    if (enemies) {
        enemies->updateSnapshot((int)grid.size(), (int)grid[0].size());
    }

    // Eğer yol yoksa veya bitti ise → yeniden bul
    if (m_path.empty() || m_stepIndex >= m_path.size()) {
        if (m_goal.r < 0 || m_goal.c < 0) return false; // hedef yok
        m_path = runAStar(grid, m_currentPos, m_goal, enemies);
        m_stepIndex = 0;
        if (m_path.empty()) return false;
    }

    Cell next = m_path[m_stepIndex];

    // düşman yüzünden geçilemiyorsa → tekrar planla
    if (!passableForLand(grid, enemies, next.r, next.c)) {
        m_path = runAStar(grid, m_currentPos, m_goal, enemies);
        m_stepIndex = 0;
        if (m_path.empty()) return false;
        next = m_path[m_stepIndex];
    }

    // adımı uygula
    m_currentPos = next;
    m_stepIndex++;

    // tabloyu boyama
    if (table) {
        if (auto* item = table->item(next.r, next.c)) {
            item->setBackground(QColor(0, 255, 0));
            item->setIcon(VisualizationConfig::landIcon());
        }
    }

    return true;
}

// A* algoritması
std::vector<Cell> LandVehicle::runAStar(
    const FindPath::Grid& grid,
    Cell start,
    Cell goal,
    const EnemyManager* enemies)
{
    struct Node {
        int r, c;
        double g, f;
        bool operator<(const Node& other) const {
            return f > other.f; // küçük olan önce gelsin
        }
    };

    std::priority_queue<Node, std::vector<Node>, std::less<Node>> open;
    std::map<Cell, double> g;
    std::map<Cell, Cell> parent;

    for (int r = 0; r < (int)grid.size(); r++) {
        for (int c = 0; c < (int)grid[0].size(); c++) {
            g[{r, c}] = std::numeric_limits<double>::infinity();
        }
    }

    g[start] = 0.0;
    open.push({ start.r, start.c, 0.0, (double)FindPath::manhattan(start, goal) });

    while (!open.empty()) {
        Node cur = open.top();
        open.pop();

        Cell u{ cur.r, cur.c };
        if (u == goal) break;

        for (int k = 0; k < 4; k++) {
            int nr = cur.r + dr[k];
            int nc = cur.c + dc[k];
            if (!passableForLand(grid, enemies, nr, nc)) continue;

            Cell v{ nr, nc };
            double newG = g[u] + 1.0;
            if (newG < g[v]) {
                g[v] = newG;
                parent[v] = u;
                double h = (double)FindPath::manhattan(v, goal);
                open.push({ nr, nc, newG, newG + h });
            }
        }
    }

    // Yol çıkarma
    std::vector<Cell> path;
    if (g[goal] == std::numeric_limits<double>::infinity())
        return path;

    Cell cur = goal;
    while (!(cur == start)) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

// Klasik findPath (tam yolu çalıştırır ve boyar)
FindPath::PathResult LandVehicle::findPath(
    const FindPath::Grid& grid,
    Cell start,
    Cell goal,
    Visualization* viz,
    double speed,
    EnemyManager* enemies)
{
    FindPath::PathResult result;
    if (grid.empty()) return result;

    m_currentPos = start;
    m_goal = goal; // game loop için de saklıyoruz
    result.nodes.push_back(start);

    int stepDelay = (int)(1000.0 / speed);

    // ilk yol
    std::vector<Cell> path = runAStar(grid, start, goal, enemies);
    if (path.empty()) {
        return result;
    }

    Cell cur = start;
    int idx = 1;

    while (!(cur == goal)) {
        if (idx >= (int)path.size()) break;
        Cell next = path[idx];

        if (enemies) {
            enemies->updateSnapshot((int)grid.size(), (int)grid[0].size());
        }

        if (!passableForLand(grid, enemies, next.r, next.c)) {
            qDebug() << "Yol kapandı, yeniden planlama!";
            path = runAStar(grid, cur, goal, enemies);
            if (path.empty()) break;
            idx = 1;
            continue;
        }

        cur = next;
        m_currentPos = cur;
        result.nodes.push_back(cur);

        if (viz) {
            if (auto table = viz->table()) {
                if (auto* item = table->item(cur.r, cur.c)) {
                    qDebug() << "cur.r:" << cur.r << " cur.c:" << cur.c;
                    item->setBackground(QColor(0, 255, 0));
                    item->setIcon(VisualizationConfig::landIcon());
                }
                QCoreApplication::processEvents();
            }
        }

        QThread::msleep(stepDelay);
        idx++;
    }

    return result;
}
