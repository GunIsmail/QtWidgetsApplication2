#include "VehicleSelectionDialog.h"

VehicleSelectionDialog::VehicleSelectionDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Ara� Tipini Se�in");

    // Checkbox'lar� olu�turma
    landCheckBox = new QCheckBox("Kara Arac�", this);
    seaCheckBox = new QCheckBox("Deniz Arac�", this);
    airCheckBox = new QCheckBox("Hava Arac�", this);

    // Butonlar� olu�turma
    okButton = new QPushButton("Tamam", this);
    cancelButton = new QPushButton("�ptal", this);

    // Buton ba�lant�lar�
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // Layout olu�turma
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