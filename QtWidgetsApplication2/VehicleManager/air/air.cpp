#include "air.h"
#include "definitions.h"
#include "visualization.h"   // table eri�imi i�in
#include <QThread>
#include <QCoreApplication>
#include <cmath>



FindPath::PathResult AirVehicle::findPath(
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    Visualization* viz,
    double speed,
    EnemyManager*)
{
    FindPath::PathResult result;

    int dr = (goal.r > start.r) ? 1 : -1;
    int dc = (goal.c > start.c) ? 1 : -1;

    int r = start.r;
    int c = start.c;

    result.nodes.push_back(start);

    FindPath::Cell prev = start;

    while (r != goal.r || c != goal.c) {
        if (r != goal.r) r += dr;
        if (c != goal.c) c += dc;

        FindPath::Cell cell{ r, c };
        result.nodes.push_back(cell);

        if (viz) {
            auto table = viz->table(); 
            if (table) {
                // �nceki h�credeki ikonu temizle
                QTableWidgetItem* prevItem = table->item(prev.r, prev.c);
                if (prevItem) {
                    prevItem->setIcon(QIcon());

                    if (grid[prev.r][prev.c] != 3) // eger o bolgede may�n varsa boyama guvenl�k acigi
                    {
                        prevItem->setBackground(VisualizationConfig::AIR_COLOR);
                    }
                }

                // yeni h�creye u�a�� koy
                QTableWidgetItem* item = table->item(cell.r, cell.c);
                if (item) {
                    item->setIcon(VisualizationConfig::airIcon());
                }
            }

            prev = cell;

            QCoreApplication::processEvents();
            QThread::msleep(std::max(10, (int)(VisualizationConfig::STEP_DELAY_MS / speed)));
        }
    }

    // �klid algoritma mesafesi
    double dx = static_cast<double>(goal.r - start.r);
    double dy = static_cast<double>(goal.c - start.c);
    result.distance = std::sqrt(dx * dx + dy * dy);
    result.elapsedTime = result.distance / speed;

    return result;
}
