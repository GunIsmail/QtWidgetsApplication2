#include "FindAlgorithmWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QColor>

FindAlgorithmWindow::FindAlgorithmWindow(const std::vector<std::vector<int>>& matrixData, QWidget* parent)
    : QDialog(parent),
    m_matrixData(matrixData)
{
    setWindowTitle("Algoritma Penceresi");
    resize(800, 600);

    // Bileþenlerin Oluþturulmasý
    infoLabel = new QLabel("Baþlangýç ve bitiþ noktalarýný seçin.", this);
    setStartButton = new QPushButton("Baþlangýç Belirle", this);
    setEndButton = new QPushButton("Bitiþ Belirle", this);
    findPathButton = new QPushButton("Yolu Bul", this);
    matrixTable = new QTableWidget(this);
    resultsTextEdit = new QTextEdit(this);
    resultsTextEdit->setReadOnly(true);

    // Buton ve Layout Ayarlarý
    findPathButton->setEnabled(false);
    setStartButton->setCheckable(true);
    setEndButton->setCheckable(true);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(setStartButton);
    buttonLayout->addWidget(setEndButton);
    buttonLayout->addWidget(findPathButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(matrixTable);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(resultsTextEdit);
    setLayout(mainLayout);

    // Fonksiyon Çaðrýlarý
    updateTableFromMatrix();

    // Sinyal ve Slot Baðlantýlarý
    connect(matrixTable, &QTableWidget::cellClicked, this, &FindAlgorithmWindow::cellClicked);
    connect(setStartButton, &QPushButton::clicked, this, &FindAlgorithmWindow::setStartPoint);
    connect(setEndButton, &QPushButton::clicked, this, &FindAlgorithmWindow::setEndPoint);
    connect(findPathButton, &QPushButton::clicked, this, &FindAlgorithmWindow::findPath);
}

FindAlgorithmWindow::~FindAlgorithmWindow()
{
}

void FindAlgorithmWindow::updateTableFromMatrix()
{
    // ...
}

void FindAlgorithmWindow::setStartPoint()
{
    // ...
}

void FindAlgorithmWindow::setEndPoint()
{
    // ...
}

void FindAlgorithmWindow::cellClicked(int row, int col)
{
    // ...
}

void FindAlgorithmWindow::findPath()
{
    // ...
}
