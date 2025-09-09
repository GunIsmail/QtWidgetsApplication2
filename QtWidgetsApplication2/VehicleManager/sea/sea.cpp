#include "sea.h"
#include "FindPath.h"
#include "visualization.h"
#include "definitions.h"
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include <set>

FindPath::PathResult Sea::parallelSearch(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    Visualization* viz,
    double speed)
{
    FindPath::PathResult out;
    std::vector<FindPath::Cell> path;
    std::set<std::pair<int, int>> mineSet; // mayýnlarý tekrar etmeden tutmak için

    int startRow = std::min(start.r, goal.r);
    int endRow = std::max(start.r, goal.r);
    int startCol = std::min(start.c, goal.c);
    int endCol = std::max(start.c, goal.c);

    // tek hücre boyama
    auto scanCell = [&](int r, int c, QColor color, bool putShip = false) {
        if (r < 0 || c < 0 || r >= (int)grid.size() || c >= (int)grid[0].size())
            return;
        if (grid[r][c] == 1 || grid[r][c] == 0) return; // kara yasak

        if (viz) {
            auto table = viz->table();
            if (table) {
                QTableWidgetItem* item = table->item(r, c);
                if (item) {
                    item->setBackground(color);
                    if (putShip) {
                        item->setIcon(VisualizationConfig::seaIcon()); // gemi ikonu
                    }
                }
            }
        }
        if (grid[r][c] == 3) {
            mineSet.insert({ r, c });
        }
        };

    // sað ve solu ayný anda boyama
    auto scanSides = [&](int r, int c) {
        bool painted = false;
        auto table = viz ? viz->table() : nullptr;

        if (r >= 0 && r < (int)grid.size() && table) {
            // sol
            if (c - 1 >= 0 && grid[r][c - 1] != 1 && grid[r][c - 1] != 0) {
                QTableWidgetItem* item = table->item(r, c - 1);
                if (item) {
                    item->setBackground(VisualizationConfig::SCAN_COLOR);
                }
                if (grid[r][c - 1] == 3) mineSet.insert({ r, c - 1 });
                painted = true;
            }
            // sað
            if (c + 1 < (int)grid[0].size() && grid[r][c + 1] != 1 && grid[r][c + 1] != 0) {
                QTableWidgetItem* item = table->item(r, c + 1);
                if (item) {
                    item->setBackground(VisualizationConfig::SCAN_COLOR);
                }
                if (grid[r][c + 1] == 3) mineSet.insert({ r, c + 1 });
                painted = true;
            }
        }

        if (painted) {
            QCoreApplication::processEvents();
            QThread::msleep(std::max(10, (int)(VisualizationConfig::STEP_DELAY_MS / speed)));
        }
        };

    // === Dikey zigzag (öncelik yukarý-aþaðý hareketler) ===
    for (int j = startCol; j <= endCol; j++) {
        if ((j - startCol) % 2 == 0) {
            // yukarýdan aþaðýya
            for (int i = startRow; i <= endRow; i++) {
                if (grid[i][j] == 1 || grid[i][j] == 0) continue;

                path.push_back({ i, j });
                if (viz) {
                    scanCell(i, j, VisualizationConfig::SEA_COLOR, true); // gemi burada
                    scanSides(i, j);
                }
            }
        }
        else {
            // aþaðýdan yukarýya
            for (int i = endRow; i >= startRow; i--) {
                if (grid[i][j] == 1 || grid[i][j] == 0) continue;

                path.push_back({ i, j });
                if (viz) {
                    scanCell(i, j, VisualizationConfig::SEA_COLOR, true); // gemi burada
                    scanSides(i, j);
                }
            }
        }
    }

    out.nodes = path;
    out.distance = static_cast<int>(path.size()) - 1;

    // mayýn listesini set'ten aktar
    for (auto& m : mineSet) {
        out.mines.push_back({ m.first, m.second });
    }

    return out;
}
