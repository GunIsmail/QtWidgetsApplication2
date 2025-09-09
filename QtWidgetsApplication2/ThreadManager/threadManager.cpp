#include "threadmanager.h"
#include "land\land.h"
#include "sea\sea.h"
#include "air\air.h"
#include <QtConcurrent>

ThreadManager::ThreadManager(QObject* parent) : QObject(parent) {}

void ThreadManager::runLand(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal, QTableWidget* table) {
    QtConcurrent::run([=]() {
        Visualization viz(table);
        auto res = Land::aStar(grid, start, goal, &viz);
        emit landFinished(res);
        });
}

void ThreadManager::runSea(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal, QTableWidget* table) {
    QtConcurrent::run([=]() {
        Visualization viz(table);
        auto res = Sea::parallelSearch(grid, start, goal, &viz);
        emit seaFinished(res);
        });
}

void ThreadManager::runAir(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal, QTableWidget* table) {
    QtConcurrent::run([=]() {
        Visualization viz(table);
        auto res = Air::findPath(grid, start, goal, &viz);
        emit airFinished(res);
        });
}
