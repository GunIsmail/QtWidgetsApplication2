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
    // Arayuz bilesenlerini olusturma
    nLabel = new QLabel("Satir  (N):", this);
    mLabel = new QLabel("Sutun  (M):", this);
    nLineEdit = new QLineEdit(this);
    mLineEdit = new QLineEdit(this);
    createButton = new QPushButton("Matris olustur", this);
    loadButton = new QPushButton("Matris Yukle", this);
    addObstacleButton = new QPushButton("Engel Ekle", this);
    addSeaButton = new QPushButton("Deniz Ekle", this);
    addMineButton = new QPushButton("Mayin Ekle", this);
    saveButton = new QPushButton("Kaydet", this);
    matrixTable = new QTableWidget(this);
    findAlgorithmButton = new QPushButton("Algoritma olustur", this);

    // Algoritma modu icin yeni bilesenler
    setStartButton = new QPushButton("Baslangic ", this);
    setEndButton = new QPushButton("Bitis ", this);
    findPathButton = new QPushButton("Yolu Bul", this);
    resetButton = new QPushButton("Geri Don", this);
    resultsTextEdit = new QTextEdit(this);
    infoLabel = new QLabel("Baslangic ve bitis sec.", this);

    // Baslangic durumu ve buton ayarlari
    currentState = AppState::None;
    addObstacleButton->setCheckable(true);
    addSeaButton->setCheckable(true);
    addMineButton->setCheckable(true);
    addObstacleButton->setEnabled(false);
    addSeaButton->setEnabled(false);
    addMineButton->setEnabled(false);
    saveButton->setEnabled(false);
    findAlgorithmButton->setEnabled(false);
    setStartButton->setCheckable(true);
    setEndButton->setCheckable(true);
    findPathButton->setEnabled(false);

    // Duðmeleri ayri yatay layout'lara yerlestirme
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
    actionButtonLayout->addWidget(findAlgorithmButton);

    // Algoritma ile ilgili bilesenlerin layout'u
    QHBoxLayout* algorithmButtonsLayout = new QHBoxLayout();
    algorithmButtonsLayout->addWidget(setStartButton);
    algorithmButtonsLayout->addWidget(setEndButton);
    algorithmButtonsLayout->addWidget(findPathButton);

    QVBoxLayout* algorithmLayout = new QVBoxLayout();
    algorithmLayout->addWidget(infoLabel);
    algorithmLayout->addLayout(algorithmButtonsLayout);
    algorithmLayout->addWidget(resultsTextEdit);
    algorithmLayout->addWidget(resetButton);

    // Tum layout'lari yöneten ana layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(matrixTable);
    mainLayout->addLayout(actionButtonLayout);
    mainLayout->addLayout(algorithmLayout,0);

    // Baslangicta algoritma ile ilgili her seyi gizle
    infoLabel->hide();
    setStartButton->hide();
    setEndButton->hide();
    findPathButton->hide();
    resultsTextEdit->hide();
    resetButton->hide();

    // Ana layout'u pencereye ayarla
    setLayout(mainLayout);
    setWindowTitle("Matrix Uygulamasi");

    // Sinyal ve slot baðlantilari
    connect(createButton, &QPushButton::clicked, this, &QtWidgetsApplication2::createMatrix);
    connect(loadButton, &QPushButton::clicked, this, &QtWidgetsApplication2::loadMatrix);
    connect(addObstacleButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addObstacleClicked);
    connect(addSeaButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addSeaClicked);
    connect(addMineButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addMineClicked);
    connect(matrixTable, &QTableWidget::cellClicked, this, &QtWidgetsApplication2::cellClicked);
    connect(saveButton, &QPushButton::clicked, this, &QtWidgetsApplication2::saveMatrix);
    connect(findAlgorithmButton, &QPushButton::clicked, this, &QtWidgetsApplication2::findAlgorithm);

    // Algoritma modu butonlari baðlantilari
    connect(setStartButton, &QPushButton::clicked, this, &QtWidgetsApplication2::setStartPoint);
    connect(setEndButton, &QPushButton::clicked, this, &QtWidgetsApplication2::setEndPoint);
    connect(findPathButton, &QPushButton::clicked, this, &QtWidgetsApplication2::findPath);
    connect(resetButton, &QPushButton::clicked, this, &QtWidgetsApplication2::resetUI);
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

    // Hem tabloyu hem m_matrixData’yi doldur
    m_matrixData.clear();
    m_matrixData.resize(n, std::vector<int>(m, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem("Kara");
            item->setBackground(QColor(139, 69, 19));
            item->setForeground(QColor(0, 0, 0));
            matrixTable->setItem(i, j, item);

            m_matrixData[i][j] = 0; // Kara
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
    findAlgorithmButton->setEnabled(true);
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

    // 1) Deniz ekleme
    if (currentState == AppState::AddingSea) {
        item->setText("Deniz");
        item->setBackground(QColor(0, 191, 255));
        item->setForeground(QColor(0, 0, 0));
        return;
    }

    // 2) Engel ekleme
    if (currentState == AppState::AddingObstacle) {
        item->setText("Engel");
        item->setBackground(QColor(0, 0, 0));
        item->setForeground(QColor(255, 255, 255));
        return;
    }

    // 3) Mayin ekleme (sadece denize)
    if (currentState == AppState::AddingMine) {
        if (item->text() == "Deniz" || item->text() == "X") {
            item->setText("X");
            item->setBackground(QColor(0, 191, 255));
            item->setForeground(QColor(255, 0, 0));
        }
        else {
            QMessageBox::warning(this, "Hata", "Mayin sadece denize yerlestirilebilir.");
        }
        return;
    }

    // 4) Baslangic secimi
    if (currentState == AppState::SettingStart) {
       
        if (item->text() == "Engel" || item->text() == "X") {
            QMessageBox::warning(this, "Uyari", "ERRRRORRRR.");
            return;
        }

        // Eski baslangici eski rengine döndur
        if (startPointItem) {
            if (startPointItem->text() == "Deniz" || startPointItem->text() == "X") {
                startPointItem->setBackground(QColor(0, 191, 255));
                if (startPointItem->text() == "X") startPointItem->setForeground(QColor(255, 0, 0));
            }
            else if (startPointItem->text() == "Engel") {
                startPointItem->setBackground(QColor(0, 0, 0));
                startPointItem->setForeground(QColor(255, 255, 255));
            }
            else {
                // Kara
                startPointItem->setBackground(QColor(139, 69, 19));
                startPointItem->setForeground(QColor(0, 0, 0));
            }
        }

        // Yeni baslangic
        item->setBackground(QColor(0, 255, 0)); // Yesil
        item->setForeground(QColor(0, 0, 0));
        startPointItem = item;
        startRow = row;
        startCol = col;

        setStartButton->setChecked(false);
        currentState = AppState::None;
        infoLabel->setText("Baslangic secildi.");
        if (endPointItem) findPathButton->setEnabled(true);
        return;
    }

    // 5) Bitis secimi
    if (currentState == AppState::SettingEnd) {
        if (item->text() == "Engel" || item->text() == "X") {
            QMessageBox::warning(this, "Uyari", "ERRRORRR.");
            return;
        }

        // Eski bitisi eski rengine döndur
        if (endPointItem) {
            if (endPointItem->text() == "Deniz" || endPointItem->text() == "X") {
                endPointItem->setBackground(QColor(0, 191, 255));
                if (endPointItem->text() == "X") endPointItem->setForeground(QColor(255, 0, 0));
            }
            else if (endPointItem->text() == "Engel") {
                endPointItem->setBackground(QColor(0, 0, 0));
                endPointItem->setForeground(QColor(255, 255, 255));
            }
            else {
                // Kara
                endPointItem->setBackground(QColor(139, 69, 19));
                endPointItem->setForeground(QColor(0, 0, 0));
            }
        }

        // Yeni bitis
        item->setBackground(QColor(255, 0, 0)); // Kirmizi
        item->setForeground(QColor(255, 255, 255));
        endPointItem = item;
        endRow = row;
        endCol = col;

        setEndButton->setChecked(false);
        currentState = AppState::None;
        infoLabel->setText("Bitis secildi.");
        if (startPointItem) findPathButton->setEnabled(true);
        return;
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
                else if (text == "X") { 
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
    m_matrixData.clear(); // Mevcut m_matrixData'yi temizle
    int cols = 0;

    // Dosyanin tum satirlarini okuyup veriyi m_matrixData'ya al
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue; // Bos satirlari atla

        QStringList values = line.split(" ", Qt::SkipEmptyParts);
        std::vector<int> row;
        for (const QString& val : values) {
            row.push_back(val.toInt());
        }
        m_matrixData.push_back(row); 

        // Sutun sayisini ilk satirdan al
        if (cols == 0) {
            cols = row.size();
        }
    }

    file.close();

    int rows = m_matrixData.size();

    // Boyutlarin gecerliliðini kontrol et
    if (rows <= 0 || cols <= 0) {
        QMessageBox::warning(this, "Hata", "Dosyada gecerli matris verisi bulunamadi.");
        return;
    }

    // QTableWidget'i okunan boyutlara göre ayarla
    matrixTable->setRowCount(rows);
    matrixTable->setColumnCount(cols);

    // m_matrixData'daki verileri QTableWidget'a aktar
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int value = m_matrixData[i][j];
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
                item->setForeground(QColor(255, 0, 0));
            }
            matrixTable->setItem(i, j, item);
        }
    }

    // Butonlari aktif et
    addObstacleButton->setEnabled(true);
    addSeaButton->setEnabled(true);
    addMineButton->setEnabled(true);
    saveButton->setEnabled(true);
    findAlgorithmButton->setEnabled(true);
}
void QtWidgetsApplication2::findAlgorithm()
{
    // Matris olusturulmadiysa hata ver
    if (m_matrixData.empty() || m_matrixData[0].empty()) {
        QMessageBox::warning(this, "Hata", "Lutfen önce bir matris olusturun veya yukleyin.");
        return;
    }

    // Matris duzenleme bilesenlerini gizle
    nLabel->hide();
    mLabel->hide();
    nLineEdit->hide();
    mLineEdit->hide();
    createButton->hide();
    loadButton->hide();
    addObstacleButton->hide();
    addSeaButton->hide();
    addMineButton->hide();
    saveButton->hide();
    findAlgorithmButton->hide();

    // Algoritma bilesenlerini göster
    infoLabel->show();
    setStartButton->show();
    setEndButton->show();
    findPathButton->show();
    resultsTextEdit->show();
    resetButton->show();


    // Secim butonlarini sifirla
    if (startPointItem) {
        // startPointItem rengini eski haline getir
    }
    if (endPointItem) {
        // endPointItem rengini eski haline getir
    }
    startPointItem = nullptr;
    endPointItem = nullptr;
    findPathButton->setEnabled(false);
    infoLabel->setText("Baslangic ve Bitis noktalarini secin ");
    resultsTextEdit->clear();
}
void QtWidgetsApplication2::setStartPoint()
{
    // Önce bitis secim modunu kapatalim
    setEndButton->setChecked(false);

    if (setStartButton->isChecked()) {
        currentState = AppState::SettingStart;
        infoLabel->setText("baslangic sec.");
    }
    else {
        currentState = AppState::None;
        infoLabel->setText("Baslangic secim iptal.");
    }
}

void QtWidgetsApplication2::setEndPoint()
{
    // Önce baslangic secim modunu kapatalim
    setStartButton->setChecked(false);

    if (setEndButton->isChecked()) {
        currentState = AppState::SettingEnd;
        infoLabel->setText("Bitis .");
    }
    else {
        currentState = AppState::None;
        infoLabel->setText("Bitis iptal .");
    }
}

void QtWidgetsApplication2::findPath()
{
    QString startText = QString("Baslangic: (%1, %2)").arg(startRow).arg(startCol);
    QString endText = QString("Bitis    : (%1, %2)").arg(endRow).arg(endCol);
    printf("%d,%d", startRow, startCol);

    if (resultsTextEdit) {
        resultsTextEdit->append(startText);
        resultsTextEdit->append(endText);
    }
    else {
        QMessageBox::information(this, "Koordinatlar", startText + "\n" + endText);
    }

    
    FindPath::Cell start_cell{ startRow, startCol };
    FindPath::Cell end_cell{ endRow, endCol };

    auto path = FindPath::AStar(m_matrixData, start_cell, end_cell);

    if (path.empty()) {
        if (resultsTextEdit) resultsTextEdit->append("Yol bulunamadi.");
        else QMessageBox::information(this, "Sonuc", "Yol bulunamadi.");
        return;
    }

    
    if (resultsTextEdit) {
        resultsTextEdit->append(QString("Yol uzunlugu: %1").arg(path.size()));
    }
}

void QtWidgetsApplication2::resetUI()
{
	// 
}