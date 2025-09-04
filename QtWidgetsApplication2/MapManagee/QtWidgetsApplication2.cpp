#include "QtWidgetsApplication2.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QColor>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <vector>
#include <QStringList>

QtWidgetsApplication2::QtWidgetsApplication2(QWidget* parent)
    : QWidget(parent)
{
    // Arayüz bileþenlerini oluþturma
    nLabel = new QLabel("Satir Sayisi (N):", this);
    mLabel = new QLabel("Sutun Sayisi (M):", this);
    nLineEdit = new QLineEdit(this);
    mLineEdit = new QLineEdit(this);
    createButton = new QPushButton("Matris Olustur", this);
    loadButton = new QPushButton("Matris Yukle", this);
    addObstacleButton = new QPushButton("Engel Ekle", this);
    addSeaButton = new QPushButton("Deniz Ekle", this);
    addMineButton = new QPushButton("Mayin Ekle", this);
    saveButton = new QPushButton("Kaydet", this);
    matrixTable = new QTableWidget(this);

    // Baþlangýç durumu ve buton ayarlarý
    currentState = AppState::None;
    addObstacleButton->setCheckable(true);
    addSeaButton->setCheckable(true);
    addMineButton->setCheckable(true);
    addObstacleButton->setEnabled(false);
    addSeaButton->setEnabled(false);
    addMineButton->setEnabled(false);
    saveButton->setEnabled(false);

    // Düðmeleri ayrý yatay layout'lara yerleþtirme
    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputLayout->addWidget(nLabel);
    inputLayout->addWidget(nLineEdit);
    inputLayout->addWidget(mLabel);
    inputLayout->addWidget(mLineEdit);
    inputLayout->addWidget(createButton);
    inputLayout->addWidget(loadButton);

    QHBoxLayout* actionButtonLayout = new QHBoxLayout();
    actionButtonLayout->addWidget(addObstacleButton);
    actionButtonLayout->addWidget(addSeaButton);
    actionButtonLayout->addWidget(addMineButton);
    actionButtonLayout->addWidget(saveButton);

    // Ana dikey layout'u oluþturma ve bileþenleri ekleme
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(matrixTable);
    mainLayout->addLayout(actionButtonLayout);

    // Ana layout'u pencereye ayarla
    setLayout(mainLayout);
    setWindowTitle("Matris Uygulamasi");

    // Sinyal ve slot baðlantýlarý
    connect(createButton, &QPushButton::clicked, this, &QtWidgetsApplication2::createMatrix);
    connect(loadButton, &QPushButton::clicked, this, &QtWidgetsApplication2::loadMatrix);
    connect(addObstacleButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addObstacleClicked);
    connect(addSeaButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addSeaClicked);
    connect(addMineButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addMineClicked);
    connect(matrixTable, &QTableWidget::cellClicked, this, &QtWidgetsApplication2::cellClicked);
    connect(saveButton, &QPushButton::clicked, this, &QtWidgetsApplication2::saveMatrix);
}

QtWidgetsApplication2::~QtWidgetsApplication2()
{
}

void QtWidgetsApplication2::createMatrix()
{
    bool okN, okM;
    int n = nLineEdit->text().toInt(&okN);
    int m = mLineEdit->text().toInt(&okM);

    if (!okN || !okM || n <= 0 || m <= 0) {
        QMessageBox::warning(this, "Hata", "Lutfen gecerli pozitif tam sayilar girin.");
        return;
    }

    matrixTable->setRowCount(n);
    matrixTable->setColumnCount(m);

    int cellSize = 50;
    for (int i = 0; i < n; ++i) {
        matrixTable->setRowHeight(i, cellSize);
    }
    for (int j = 0; j < m; ++j) {
        matrixTable->setColumnWidth(j, cellSize);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem("Kara");
            item->setBackground(QColor(139, 69, 19));
            item->setForeground(QColor(0, 0, 0));
            matrixTable->setItem(i, j, item);
        }
    }

    matrixTable->horizontalHeader()->hide();
    matrixTable->verticalHeader()->hide();

    int tableWidth = m * cellSize + matrixTable->verticalHeader()->width();
    int tableHeight = n * cellSize + matrixTable->horizontalHeader()->height();
    int windowWidth = tableWidth + 50;
    int windowHeight = tableHeight + 150;
    this->resize(windowWidth, windowHeight);

    addObstacleButton->setEnabled(true);
    addSeaButton->setEnabled(true);
    addMineButton->setEnabled(true);
    saveButton->setEnabled(true);
}

void QtWidgetsApplication2::addObstacleClicked()
{
    addSeaButton->setChecked(false);
    addMineButton->setChecked(false);
    if (addObstacleButton->isChecked()) {
        currentState = AppState::AddingObstacle;
    }
    else {
        currentState = AppState::None;
    }
}

void QtWidgetsApplication2::addSeaClicked()
{
    addObstacleButton->setChecked(false);
    addMineButton->setChecked(false);
    if (addSeaButton->isChecked()) {
        currentState = AppState::AddingSea;
    }
    else {
        currentState = AppState::None;
    }
}

void QtWidgetsApplication2::addMineClicked()
{
    addObstacleButton->setChecked(false);
    addSeaButton->setChecked(false);
    if (addMineButton->isChecked()) {
        currentState = AppState::AddingMine;
    }
    else {
        currentState = AppState::None;
    }
}

void QtWidgetsApplication2::cellClicked(int row, int col)
{
    QTableWidgetItem* item = matrixTable->item(row, col);
    if (!item) return;

    if (currentState == AppState::AddingSea) {
        item->setText("Deniz");
        item->setBackground(QColor(0, 191, 255));
        item->setForeground(QColor(0, 0, 0));
    }
    else if (currentState == AppState::AddingObstacle) {
        item->setText("Engel");
        item->setBackground(QColor(0, 0, 0));
        item->setForeground(QColor(255, 255, 255));
    }
    else if (currentState == AppState::AddingMine) {
        // Mayýn sadece denize yerleþtirilebilir.
        if (item->text() == "Deniz" || item->text() == "X") { // "X" olan hücrelere de mayýn eklenmesine izin ver.
            item->setText("X"); // Sembolü X olarak ayarla
            item->setBackground(QColor(0, 191, 255));
            item->setForeground(QColor(255, 0, 0)); // Kýrmýzý rengi ayarla
        }
        else {
            QMessageBox::warning(this, "Hata", "Mayin sadece denize yerlestirilebilir.");
        }
    }
}

void QtWidgetsApplication2::saveMatrix()
{
    QString filePathToSave = m_currentFilePath;

    if (filePathToSave.isEmpty()) {
        filePathToSave = QFileDialog::getSaveFileName(this, "Matrisi Kaydet", "", "Metin Dosyalari (*.txt);;Tum Dosyalar (*)");
    }

    if (filePathToSave.isEmpty()) {
        return;
    }

    QFile file(filePathToSave);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Hata", "Dosya yazma hatasi: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    int rows = matrixTable->rowCount();
    int cols = matrixTable->columnCount();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QTableWidgetItem* item = matrixTable->item(i, j);
            int value = 0;
            if (item) {
                QString text = item->text();
                if (text == "Engel") {
                    value = 1;
                }
                else if (text == "Deniz") {
                    value = 2;
                }
                else if (text == "X") { // "Mayin" yerine "X" olarak kontrol et
                    value = 3;
                }
            }
            out << value;
            if (j < cols - 1) {
                out << " ";
            }
        }
        out << "\n";
    }

    file.close();
    QMessageBox::information(this, "Basarili", "Matris basariyla kaydedildi.");

    if (m_currentFilePath.isEmpty()) {
        m_currentFilePath = filePathToSave;
    }
}

void QtWidgetsApplication2::loadMatrix()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Matris Yukle", "", "Metin Dosyalari (*.txt);;Tum Dosyalar (*)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Hata", "Dosya okuma hatasi: " + file.errorString());
        return;
    }

    m_currentFilePath = fileName;

    QTextStream in(&file);
    std::vector<std::vector<int>> matrixData;
    int cols = 0;

    // Dosyanýn tüm satýrlarýný okuyup veriyi bir vektöre al
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue; // Boþ satýrlarý atla

        QStringList values = line.split(" ", Qt::SkipEmptyParts);
        std::vector<int> row;
        for (const QString& val : values) {
            row.push_back(val.toInt());
        }
        matrixData.push_back(row);

        // Sütun sayýsýný ilk satýrdan al
        if (cols == 0) {
            cols = row.size();
        }
    }

    file.close();

    int rows = matrixData.size();

    // Boyutlarýn geçerliliðini kontrol et
    if (rows <= 0 || cols <= 0) {
        QMessageBox::warning(this, "Hata", "Dosyada gecerli matris verisi bulunamadi.");
        return;
    }

    // QTableWidget'ý okunan boyutlara göre ayarla
    matrixTable->setRowCount(rows);
    matrixTable->setColumnCount(cols);

    // Vektördeki verileri QTableWidget'a aktar
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int value = matrixData[i][j];
            QTableWidgetItem* item = new QTableWidgetItem();

            if (value == 0) {
                item->setText("Kara");
                item->setBackground(QColor(139, 69, 19));
                item->setForeground(QColor(0, 0, 0));
            }
            else if (value == 1) {
                item->setText("Engel");
                item->setBackground(QColor(0, 0, 0));
                item->setForeground(QColor(255, 255, 255));
            }
            else if (value == 2) {
                item->setText("Deniz");
                item->setBackground(QColor(0, 191, 255));
                item->setForeground(QColor(0, 0, 0));
            }
            else if (value == 3) {
                item->setText("X");
                item->setBackground(QColor(0, 191, 255));
                item->setForeground(QColor(255, 0, 0)); // Kýrmýzý rengi burada doðru ayarla
            }
            matrixTable->setItem(i, j, item);
        }
    }

    // Butonlarý etkinleþtir
    addObstacleButton->setEnabled(true);
    addSeaButton->setEnabled(true);
    addMineButton->setEnabled(true);
    saveButton->setEnabled(true);

    QMessageBox::information(this, "Basarili", "Matris basariyla yuklendi.");
}