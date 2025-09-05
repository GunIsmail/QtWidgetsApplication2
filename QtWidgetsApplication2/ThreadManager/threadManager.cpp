#include "threadManager.h"
#include "../VehicleManager/land/land.h"
#include "../VehicleManager/sea/sea.h"
#include "../VehicleManager/air/air.h"
#include <QtConcurrent>

ThreadManager::ThreadManager(QObject* parent) : QObject(parent) {}

void ThreadManager::runLand(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal) {
    QtConcurrent::run([=]() {
        FindPath::PathResult result = Land::aStar(grid, start, goal);
        emit landFinished(result);
        });
}

void ThreadManager::runSea(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal) {
	//todo buraya deniz yolu algoritmasi eklenecek
}

void ThreadManager::runAir(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal) {
	//todo buraya hava yolu algoritmasi eklenecek
    
    QtConcurrent::run([=]() {
        // air::calculatePath fonksiyonunu çaðýr
        FindPath::PathResult result = Air::findPath (grid, start, goal);
        emit airFinished(result);
        });
}
