#ifndef VEHICLESELECTIONDIALOG_H
#define VEHICLESELECTIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>

// Ana uygulaman�z�n ba�l� oldu�u FindPath::Vehicle enum'unu dahil edin
#include "FindPath.h" 

class VehicleSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VehicleSelectionDialog(QWidget* parent = nullptr);
    QList<FindPath::Vehicle> getSelectedVehicles() const;

private:
    QCheckBox* landCheckBox;
    QCheckBox* seaCheckBox;
    QCheckBox* airCheckBox;
    QPushButton* okButton;
    QPushButton* cancelButton;
};

#endif // VEHICLESELECTIONDIALOG_H