#ifndef VEHICLESELECTIONDIALOG_H
#define VEHICLESELECTIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "vehicle/vehicle.h"
#include "FindPath.h" 
#include "VehicleManager/land/land.h"
#include "VehicleManager/sea/sea.h"
#include "VehicleManager/air/air.h"

class VehicleSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VehicleSelectionDialog(QWidget* parent = nullptr);
    QList<Vehicle*> getSelectedVehicles() const;

private:
    QCheckBox* landCheckBox;
    QCheckBox* seaCheckBox;
    QCheckBox* airCheckBox;
    QPushButton* okButton;
    QPushButton* cancelButton;
};

#endif // VEHICLESELECTIONDIALOG_H