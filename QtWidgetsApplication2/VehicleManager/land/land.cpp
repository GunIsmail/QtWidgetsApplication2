#include "land.h"
#include "definitions.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

// A* için struct yapisi
struct Node {
    int r, c;
    int g;
    int f;
};

struct ByF {
    bool operator()(const Node& a, const Node& b) const {
        return a.f > b.f;
    }
};

// Hücre kara mı kontrol et
static bool passableForLand(const FindPath::Grid& grid, int r, int c) {
    if (!FindPath::inBounds(r, c, (int)grid.size(), (int)grid[0].size()))
        return false;
    return grid[r][c] == 0; // 0 = Kara
}

FindPath::PathResult LandVehicle::findPath(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    Visualization* viz,
    double speed)
{
    FindPath::PathResult out;
    int R = (int)grid.size();
    if (R == 0) return out;
    int C = (int)grid[0].size();

    if (!passableForLand(grid, start.r, start.c) ||
        !passableForLand(grid, goal.r, goal.c))
        return out;

    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    std::priority_queue<Node, std::vector<Node>, ByF> open;
    std::vector<std::vector<int>> g(R, std::vector<int>(C, std::numeric_limits<int>::max()));
    std::vector<std::vector<bool>> closed(R, std::vector<bool>(C, false));
    std::vector<std::vector<FindPath::Cell>> parent(R, std::vector<FindPath::Cell>(C, { -1, -1 }));

    g[start.r][start.c] = 0;
    open.push({ start.r, start.c, 0, FindPath::manhattan(start, goal) });

    while (!open.empty()) {
        Node cur = open.top(); open.pop();
        if (closed[cur.r][cur.c]) continue;
        closed[cur.r][cur.c] = true;


        if (cur.r == goal.r && cur.c == goal.c) {
            std::vector<FindPath::Cell> revNodes;
            FindPath::Cell p = goal;
            while (!(p == start)) {
                revNodes.push_back(p);
                p = parent[p.r][p.c];
            }
            revNodes.push_back(start);
            std::reverse(revNodes.begin(), revNodes.end());
            out.nodes = revNodes;
            out.distance = out.nodes.size() - 1;

            if (viz) {
                auto table = viz->table();
                if (table) {
                    FindPath::Cell prev = start;

                    for (auto& cell : out.nodes) {
                        // önceki hücreyi temizle
                        if (!(cell == start)) {
                            QTableWidgetItem* prevItem = table->item(prev.r, prev.c);
                            if (prevItem) {
                                prevItem->setIcon(QIcon());
                                prevItem->setBackground(VisualizationConfig::LAND_COLOR);
                            }
                        }

                        // yeni hücreye tank koy
                        QTableWidgetItem* item = table->item(cell.r, cell.c);
                        if (item) {
                            item->setIcon(VisualizationConfig::landIcon()); // tank
                        }

                        prev = cell;
                        QCoreApplication::processEvents();
                        QThread::msleep(std::max(10, (int)(VisualizationConfig::STEP_DELAY_MS / Speed::land)));
                    }
                }
            }
            return out;
        }

        for (int k = 0; k < 4; k++) {
            int nr = cur.r + dr[k];
            int nc = cur.c + dc[k];
            if (!passableForLand(grid, nr, nc) || closed[nr][nc]) continue;

            int tentative_g = g[cur.r][cur.c] + 1;
            if (tentative_g < g[nr][nc]) {
                g[nr][nc] = tentative_g;
                parent[nr][nc] = { cur.r, cur.c };
                int f = tentative_g + FindPath::manhattan({ nr, nc }, goal);
                open.push({ nr, nc, tentative_g, f });
            }
        }
    }

    return out;
}