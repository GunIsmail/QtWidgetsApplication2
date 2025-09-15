#include "land.h"
#include "definitions.h"
#include <queue>
#include <map>
#include <set>
#include <limits>
#include <cmath>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

//Kara aracı dinamik düşman konumlarını dikkate alarak güvenli ve en kısa yolu bulur. 
// Algoritma: D* Lite / LPA* mantığıyla hedef düğümden başlayarak maliyet değerlerini (g ve rhs) hesaplar, başlangıçtan hedefe doğru en kısa yol seçilir.


static const int dr[4] = { -1, 1, 0, 0 };
static const int dc[4] = { 0, 0, -1, 1 };

// Hücre geçilebilir mi (kara + düşman kontrolü)
static bool passableForLand(const FindPath::Grid& grid, const EnemyManager* enemies, int r, int c) {
	int safeRadius = 1; // düşman etrafında güvenlik alanı
    if (!FindPath::inBounds(r, c, (int)grid.size(), (int)grid[0].size()))
        return false;
    if (grid[r][c] != 0) return false;       // 0 = kara

    if (enemies) {
        // Düşman etrafında güvenlik alanı kontrolü
        for (int dr = -safeRadius; dr <= safeRadius; dr++) {
            for (int dc = -safeRadius; dc <= safeRadius; dc++) {
                int nr = r + dr;
                int nc = c + dc;
                if (FindPath::inBounds(nr, nc, (int)grid.size(), (int)grid[0].size())) {
                    if (enemies->isOccupied(nr, nc)) {
                        return false; // güvenlik alanına girme
                    }
                }
            }
        }
    }

    return true;
}



FindPath::PathResult LandVehicle::findPath(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    Visualization* viz,
    double speed)
{
    FindPath::PathResult result;

    int R = grid.size();
    if (R == 0) return result;
    int C = grid[0].size();

    // g ve rhs tabloları
    std::map<std::pair<int, int>, double> g, rhs;
    auto key = [&](int r, int c) { return std::make_pair(r, c); };

    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            g[key(r, c)] = std::numeric_limits<double>::infinity();
            rhs[key(r, c)] = std::numeric_limits<double>::infinity();
        }
    }

    rhs[key(goal.r, goal.c)] = 0.0;

    // öncelik kuyruğu
    struct NodeCompare {
        bool operator()(const std::pair<double, std::pair<int, int>>& a,
            const std::pair<double, std::pair<int, int>>& b) const {
            if (a.first == b.first)
                return a.second < b.second;
            return a.first > b.first; // küçük öncelikli
        }
    };

    std::priority_queue<
        std::pair<double, std::pair<int, int>>,
        std::vector<std::pair<double, std::pair<int, int>>>,
        NodeCompare
    > pq;

    pq.push({ 0.0, key(goal.r, goal.c) });

    // D* Lite benzeri işlem
    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();
        int r = current.second.first;
        int c = current.second.second;

        double old_g = g[key(r, c)];
        double new_g = rhs[key(r, c)];
        if (old_g != new_g) {
            g[key(r, c)] = new_g;
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (passableForLand(grid, enemyManager, nr, nc)) {
                    double cost = 1.0;
                    double val = g[key(r, c)] + cost;
                    if (val < rhs[key(nr, nc)]) {
                        rhs[key(nr, nc)] = val;
                        pq.push({ val + std::hypot(goal.r - nr, goal.c - nc), key(nr, nc) });
                    }
                }
            }
        }
    }

   
    FindPath::Cell cur = start;
    result.nodes.push_back(cur);

    while (!(cur.r == goal.r && cur.c == goal.c)) {
        double best = std::numeric_limits<double>::infinity();
        FindPath::Cell next = cur;
        for (int k = 0; k < 4; k++) {
            int nr = cur.r + dr[k], nc = cur.c + dc[k];
            if (passableForLand(grid, enemyManager, nr, nc)) {
                if (g[key(nr, nc)] < best) {
                    best = g[key(nr, nc)];
                    next = { nr, nc };
                }
            }
        }
        if (next.r == cur.r && next.c == cur.c) {
            qDebug() << "Yol bulunamadi";
            break;
        }

        result.nodes.push_back(next);

      
        if (viz) {
            auto table = viz->table();
            if (table) {
                // önceki hücreyi temizle
                QTableWidgetItem* prevItem = table->item(cur.r, cur.c);
                if (prevItem) {
                    prevItem->setIcon(QIcon());
                    prevItem->setBackground(VisualizationConfig::LAND_COLOR);
                }

                // yeni hücreye tank koy
                QTableWidgetItem* item = table->item(next.r, next.c);
                if (item) {
                    item->setIcon(VisualizationConfig::landIcon());
                }
                //  Düşman ile güvenlik alanı çakışma kontrolü
                if (enemyManager) {
                    int safeRadius = 1; //guvenlık yaricapi 

                    for (int dr = -safeRadius; dr <= safeRadius; dr++) {
                        for (int dc = -safeRadius; dc <= safeRadius; dc++) {
                            int nr = next.r + dr;
                            int nc = next.c + dc;

                            if (FindPath::inBounds(nr, nc, (int)grid.size(), (int)grid[0].size())) {
                                if (enemyManager->isOccupied(nr, nc)) {
                                    QMessageBox::critical(nullptr, "Uyarı",
                                        QString("Kara aracı düşman güvenlik alanına girdi! (%1,%2)")
                                        .arg(next.r).arg(next.c));
                                    return result;
                                }
                            }
                        }
                    }
                }

                QCoreApplication::processEvents();
				// Hız ayarı definitionstan alinir. 
                QThread::msleep( int(VisualizationConfig::STEP_DELAY_MS / Speed::land));
            }
        }
        

        cur = next;
    }

    return result;
}
