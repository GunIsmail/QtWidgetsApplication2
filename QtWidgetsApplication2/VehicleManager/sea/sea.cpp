#include "sea.h"
#include <QThread>
#include <QCoreApplication>
#include <set>
#include <QDebug>
#include <cstdlib>
#include <ctime>


FindPath::PathResult SeaVehicle::findPath(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    Visualization* viz,
    double speed,
    EnemyManager*)
{
    FindPath::PathResult out;
    std::vector<FindPath::Cell> path;
    std::set<std::pair<int, int>> mineSet; // mayınları tekrar etmeden tutmak için

    // === Noise matrisi ===
    std::vector<std::vector<int>> seaNoise(grid.size(), std::vector<int>(grid[0].size(), -1));
    std::srand((unsigned)std::time(nullptr));
    for (int r = 0; r < (int)grid.size(); r++) {
        for (int c = 0; c < (int)grid[0].size(); c++) {
            if (grid[r][c] == 2 || grid[r][c] == 3) { // deniz veya mayın
                seaNoise[r][c] = 30 + (std::rand() % 41); // 30–70 arası
            }
        }
    }

    int startRow = std::min(start.r, goal.r);
    int endRow = std::max(start.r, goal.r);
    int startCol = std::min(start.c, goal.c);
    int endCol = std::max(start.c, goal.c);

    // Tek hücre boyama
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
                        item->setIcon(VisualizationConfig::seaIcon());
                    }
                    if (seaNoise[r][c] != -1) {
                        item->setText(QString::number(seaNoise[r][c]));
                    }
                }
            }
        }

        if (grid[r][c] == 3) {
            qDebug() << "scanCell Mayin kontrolu:" << r << c
                << "noise=" << seaNoise[r][c];
            if (seaNoise[r][c] <= noiseRate::seaNoiceRate) {
                qDebug() << "   -> Mayin ALGILANDI (mineSet'e eklendi)";
                mineSet.insert({ r, c });
            }
            else {
                qDebug() << "   -> Mayin GORULMEDI (noise buyuk)";
            }
        }
        };

    // Sağ ve solu aynı anda boyama
    auto scanSides = [&](int r, int c) {
        bool painted = false;
        auto table = viz ? viz->table() : nullptr;

        if (r >= 0 && r < (int)grid.size() && table) {
            // sol
            if (c - 1 >= 0 && grid[r][c - 1] != 1 && grid[r][c - 1] != 0) {
                if (grid[r][c - 1] == 3) {
                    qDebug() << "scanSides (sol) Mayin kontrolu:" << r << c - 1
                        << "noise=" << seaNoise[r][c - 1];
                    if (seaNoise[r][c - 1] <= noiseRate::seaNoiceRate) {
                        qDebug() << "   -> ALGILANDI";
                        mineSet.insert({ r, c - 1 });
                    }
                    else {
                        qDebug() << "   -> GORULMEDI";
                    }
                }

                QTableWidgetItem* item = table->item(r, c - 1);
                if (item) {
                    item->setBackground(VisualizationConfig::SCAN_COLOR);
                    if (seaNoise[r][c - 1] != -1) {
                        item->setText(QString::number(seaNoise[r][c - 1]));
                    }
                }
                painted = true;
            }

            // sağ
            if (c + 1 < (int)grid[0].size() && grid[r][c + 1] != 1 && grid[r][c + 1] != 0) {
                if (grid[r][c + 1] == 3) {
                    qDebug() << "scanSides (sag) Mayin kontrolu:" << r << c + 1
                        << "noise=" << seaNoise[r][c + 1];
                    if (seaNoise[r][c + 1] <= noiseRate::seaNoiceRate) {
                        qDebug() << "   -> ALGILANDI";
                        mineSet.insert({ r, c + 1 });
                    }
                    else {
                        qDebug() << "   -> GORULMEDI";
                    }
                }

                QTableWidgetItem* item = table->item(r, c + 1);
                if (item) {
                    item->setBackground(VisualizationConfig::SCAN_COLOR);
                    if (seaNoise[r][c + 1] != -1) {
                        item->setText(QString::number(seaNoise[r][c + 1]));
                    }
                }
                painted = true;
            }
        }

        if (painted) {
            QCoreApplication::processEvents();
            QThread::msleep(std::max(10, (int)(VisualizationConfig::STEP_DELAY_MS / Speed::sea)));
        }
        };

    // Zigzag tarama
    for (int j = startCol; j <= endCol; j++) {
        if ((j - startCol) % 2 == 0) {
            for (int i = startRow; i <= endRow; i++) {
                if (grid[i][j] == 1 || grid[i][j] == 0) break;
                path.push_back({ i, j });
                if (viz) {
                    scanCell(i, j, VisualizationConfig::SEARCH_COLOR, true);
                    scanSides(i, j);
                }
            }
        }
        else {
            for (int i = endRow; i >= startRow; i--) {
                if (grid[i][j] == 1 || grid[i][j] == 0) break;
                path.push_back({ i, j });
                if (viz) {
                    scanCell(i, j, VisualizationConfig::SEARCH_COLOR, true);
                    scanSides(i, j);
                }
            }
        }
    }

    out.nodes = path;
    out.distance = static_cast<int>(path.size()) - 1;
    out.elapsedTime = out.distance / speed;

    // Mayın listesini setten al
    for (auto& m : mineSet) {
        out.mines.push_back({ m.first, m.second });
    }

    // Final boyama
    for (auto& m : out.mines) {
        int r = m.r;
        int c = m.c;
        qDebug() << "Final boyama:" << r << c
            << "noise=" << seaNoise[r][c];
        if (viz && viz->table() && seaNoise[r][c] <= noiseRate::seaNoiceRate) {
            qDebug() << "   -> Kirmizi + ikon basildi";
            QTableWidgetItem* item = viz->table()->item(r, c);
            if (item) {
                item->setBackground(VisualizationConfig::MINE_COLOR);
                item->setIcon(VisualizationConfig::mineIcon());
            }
        }
        else {
            qDebug() << "   -> Boyanmadı";
        }
    }

    return out;
}
