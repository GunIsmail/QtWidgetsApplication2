#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <QObject>
#include "findpath.h"
#include "visualization.h"

class ThreadManager : public QObject {
    Q_OBJECT
public:
    explicit ThreadManager(QObject* parent = nullptr);

    void runLand(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal, QTableWidget* table);
    void runSea(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal, QTableWidget* table);
    void runAir(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal, QTableWidget* table);

signals:
    void landFinished(FindPath::PathResult result);
    void seaFinished(FindPath::PathResult result);
    void airFinished(FindPath::PathResult result);
};

#endif
