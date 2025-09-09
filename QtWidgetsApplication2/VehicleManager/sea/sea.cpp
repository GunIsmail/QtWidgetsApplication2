#include "sea.h"
#include "FindPath.h"
#include "visualization.h"
#include "definitions.h"
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

FindPath::PathResult Sea::parallelSearch(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    Visualization* viz)
{
    FindPath::PathResult out;
    std::vector<FindPath::Cell> path;

    int startRow = std::min(start.r, goal.r);
    int endRow = std::max(start.r, goal.r);
    int startCol = std::min(start.c, goal.c);
    int endCol = std::max(start.c, goal.c);

    for (int i = startRow; i <= endRow; i++) {
        if ((i - startRow) % 2 == 0) {
            // soldan saða
            for (int j = startCol; j <= endCol; j++) {
                if (grid[i][j] == 1 || grid[i][j] == 0) continue; // kara yasak
                path.push_back({ i, j });
                if (grid[i][j] == 3)
                    out.mines.push_back({ i, j });

          
                if (viz) {
                    viz->visualizeStep({ i, j }, VisualizationConfig::SEA_COLOR);
                    QCoreApplication::processEvents();
                    QThread::msleep(VisualizationConfig::STEP_DELAY_MS);
                }
            }
        }
        else {
            // saðdan sola
            for (int j = endCol; j >= startCol; j--) {
                if (grid[i][j] == 1 || grid[i][j] == 0) continue;
                path.push_back({ i, j });
                if (grid[i][j] == 3)
                    out.mines.push_back({ i, j });

              
                if (viz) {
                    viz->visualizeStep({ i, j }, VisualizationConfig::SEA_COLOR);
                    QCoreApplication::processEvents();
                    QThread::msleep(VisualizationConfig::STEP_DELAY_MS);
                }
            }
        }
    }

    out.nodes = path;
    out.distance = static_cast<int>(path.size()) - 1;
    return out;
}
