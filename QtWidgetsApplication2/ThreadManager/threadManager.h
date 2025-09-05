#pragma once
#include <QObject>
#include <QThread>
#include "FindPath.h"

class ThreadManager : public QObject {
    Q_OBJECT
public:
    explicit ThreadManager(QObject* parent = nullptr);
    void runLand(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal);
    void runSea(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal);
    void runAir(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal);

signals:
    void landFinished(FindPath::PathResult result);
    void seaFinished(FindPath::PathResult result);
    void airFinished(FindPath::PathResult result);
};
