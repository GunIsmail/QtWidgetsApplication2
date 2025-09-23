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

// Komşu hareket yönleri
static const int dr[4] = { -1, 1, 0, 0 };
static const int dc[4] = { 0, 0, -1, 1 };

using Cell = FindPath::Cell;

LandVehicle::LandVehicle() {
    m_currentPos = { 0, 0 };
    m_stepIndex = 0;
    m_path.clear();
    m_travelledPath.clear();
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
    if (grid[r][c] != 0)  // sadece kara
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

// ==============================
// Tek adım ilerleme (animasyon)
// ==============================
bool LandVehicle::stepMove(
    const FindPath::Grid& grid,
    EnemyManager* enemies,
    QTableWidget* table)
{
    // ✅ Hedefe ulaşıldı mı?
    if (m_currentPos == m_goal) {
        return false;
    }

    // ✅ Yol geçerli mi?
    if (m_path.empty() || m_stepIndex >= (int)m_path.size()) {
        qDebug() << "Kara aracı için geçerli yol yok veya adım bitti.";
        return false;
    }

    Cell next = m_path[m_stepIndex];

    // ✅ Yol güvenli mi? Değilse tekrar hesapla
    if (!passableForLand(grid, enemies, next.r, next.c)) {
        m_path = runAStar(grid, m_currentPos, m_goal, enemies);
        m_stepIndex = 0;

        if (m_path.empty()) {
            qDebug() << "Kara aracı: Yol şu an kapalı, bekleniyor...";
            return true;   // 🚩 Araç bekler → düşman hareket ettikçe tekrar deneyecek
        }

        next = m_path[m_stepIndex];
    }

    //  Out of range kontrolü
    if (m_stepIndex < 0 || m_stepIndex >= (int)m_path.size()) {
        qDebug() << "HATA: m_stepIndex geçersiz!" << m_stepIndex;
        return false;
    }

    //  Adım at
    m_currentPos = next;
    m_stepIndex++;
    m_travelledPath.push_back(next);

    //  Hücre boyama
    if (table) {
        if (next.r >= 0 && next.r < table->rowCount() &&
            next.c >= 0 && next.c < table->columnCount())
        {
            if (auto* item = table->item(next.r, next.c)) {
                item->setBackground(VisualizationConfig::LAND_COLOR);
                item->setIcon(VisualizationConfig::landIcon());
            }
        }
    }

    //  Anlık animasyon gecikmesi
    QCoreApplication::processEvents();
    QThread::msleep(std::max(10, (int)(VisualizationConfig::STEP_DELAY_MS / Speed::land)));

    //  Hedef kontrolü
    if (m_currentPos == m_goal) {
        FindPath::PathResult result;
        result.nodes = m_travelledPath;
        result.distance = (int)m_travelledPath.size() - 1;
        result.elapsedTime = result.distance / Speed::land;

        emit finished(name(), result);
        return false;
    }

    return true;
}


// ==============================
// Yol bulma (başlatma)
// ==============================
FindPath::PathResult LandVehicle::findPath(
    const FindPath::Grid& grid,
    Cell start,
    Cell goal,
    Visualization*,
    double,
    EnemyManager* enemies)
{
    FindPath::PathResult result;
    if (grid.empty()) return result;

    m_currentPos = start;
    m_goal = goal;
    m_travelledPath.clear();   // 🔹 yeni arama için temizle
    m_travelledPath.push_back(start);

    // İlk A* hesapla
    m_path = runAStar(grid, start, goal, enemies);
    m_stepIndex = 0;

    if (m_path.empty()) {
        qDebug() << "Kara aracı yol bulamadı!";
        return result;
    }

    // 🔹 burada result dolmuyor, çünkü gerçek yol animasyondan sonra belli olacak
    return result;
}

// ==============================
// A* algoritması
// ==============================
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
            return f > other.f;
        }
    };

    std::priority_queue<Node> open;
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

    std::vector<Cell> path;
    if (g[goal] == std::numeric_limits<double>::infinity())
        return path;

    Cell cur = goal;
    while (!(cur == start)) {
        path.push_back(cur);
        auto it = parent.find(cur);
        if (it == parent.end()) break;
        cur = it->second;
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}
