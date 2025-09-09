#include "air.h"
#include "definitions.h"
#include <QThread>
#include <QCoreApplication>
#include <cmath>

FindPath::PathResult Air::findPath(const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    Visualization* viz)
{
    FindPath::PathResult result;

    int dr = (goal.r > start.r) ? 1 : -1;
    int dc = (goal.c > start.c) ? 1 : -1;

    int r = start.r;
    int c = start.c;

    result.nodes.push_back(start);

    while (r != goal.r || c != goal.c) {
        if (r != goal.r) r += dr;
        if (c != goal.c) c += dc;

        FindPath::Cell cell{ r, c };
        result.nodes.push_back(cell);

        if (viz) {
            viz->visualizeStep(cell, VisualizationConfig::AIR_COLOR);
            QCoreApplication::processEvents();
            QThread::msleep(VisualizationConfig::STEP_DELAY_MS);
        }
    }

    // Öklidyen mesafe
    double dx = static_cast<double>(goal.r - start.r);
    double dy = static_cast<double>(goal.c - start.c);
    result.distance = std::sqrt(dx * dx + dy * dy);

    return result;
}
