#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QThread>
#include <QObject>
#include <QTableWidget>
#include "FindPath.h"
#include "vehicle/vehicle.h"
#include "EnemyManager/EnemyManager.h"
#include "visualization.h"

// ---------------- EnemyThread ---------------- olustruldu 
class EnemyThread : public QThread {
    Q_OBJECT
public:
    EnemyThread(EnemyManager* manager, Visualization* viz, double speed);
    void run() override;

private:
    EnemyManager* m_manager;
    Visualization* m_viz;
    double m_speed;
};

// ---------------- ThreadManager ----------------
class ThreadManager : public QObject {
    Q_OBJECT
public:
    explicit ThreadManager(QObject* parent = nullptr);

    void runVehicle(Vehicle* vehicle,
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        QTableWidget* table,
        double speed, EnemyManager* enemies);

signals:
    void vehicleFinished(QString vehicleName, FindPath::PathResult result);
};

#endif // THREADMANAGER_H
