#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QTableWidget>
#include "FindPath.h"
#include "vehicle/vehicle.h"

class ThreadManager : public QObject {
    Q_OBJECT
public:
    explicit ThreadManager(QObject* parent = nullptr);

    void runVehicle(Vehicle* vehicle,
        const FindPath::Grid& grid,
        FindPath::Cell start,
        FindPath::Cell goal,
        QTableWidget* table,double speed);

signals:
    void vehicleFinished(QString vehicleName, FindPath::PathResult result);
};

#endif // THREADMANAGER_H
