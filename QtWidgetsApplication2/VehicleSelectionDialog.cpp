#include "VehicleSelectionDialog.h"

VehicleSelectionDialog::VehicleSelectionDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Araç Tipini Seçin");

    // Checkbox'larý oluþturma
    landCheckBox = new QCheckBox("Kara Aracý", this);
    seaCheckBox = new QCheckBox("Deniz Aracý", this);
    airCheckBox = new QCheckBox("Hava Aracý", this);

    // Butonlarý oluþturma
    okButton = new QPushButton("Tamam", this);
    cancelButton = new QPushButton("Ýptal", this);

    // Buton baðlantýlarý
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // Layout oluþturma
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(landCheckBox);
    mainLayout->addWidget(seaCheckBox);
    mainLayout->addWidget(airCheckBox);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);
}

QList<FindPath::Vehicle> VehicleSelectionDialog::getSelectedVehicles() const
{
    QList<FindPath::Vehicle> selectedVehicles;
    if (landCheckBox->isChecked()) {
        selectedVehicles.append(FindPath::Vehicle::Land);
    }
    if (seaCheckBox->isChecked()) {
        selectedVehicles.append(FindPath::Vehicle::Sea);
    }
    if (airCheckBox->isChecked()) {
        selectedVehicles.append(FindPath::Vehicle::Air);
    }
    return selectedVehicles;
}