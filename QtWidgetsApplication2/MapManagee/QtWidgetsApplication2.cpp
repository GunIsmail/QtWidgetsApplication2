#include "QtWidgetsApplication2.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QColor>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <vector>
#include <QStringList>
#include <QInputDialog>
#include <Qstring>
#include <QTextEdit>

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
    skipButton = new QPushButton("Atla", this);   


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
    algorithmButtonsLayout->addWidget(skipButton); // YENÝ


    QVBoxLayout* algorithmLayout = new QVBoxLayout();
    algorithmLayout->addWidget(infoLabel);
    algorithmLayout->addLayout(algorithmButtonsLayout);
    algorithmLayout->addWidget(resultsTextEdit);
    algorithmLayout->addWidget(resetButton);
    algorithmButtonsLayout->addWidget(skipButton);

    // Tum layout'lari yoneten ana layout
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
    skipButton->hide();

    // main layout pencereye ayarla
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
    connect(skipButton, &QPushButton::clicked, this, &QtWidgetsApplication2::skipVehicle);

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

    // 1) Matris Duzenleme Modu: Hangi duzenleme butonu seciliyse, hucreyi ona gore ayarlar ve veriyi gunceller.
    if (currentState == AppState::AddingSea) {
        item->setText("Deniz");
        item->setBackground(QColor(0, 191, 255));
        item->setForeground(QColor(0, 0, 0));
        m_matrixData[row][col] = 2; // Deniz: 2
        return;
    }
    if (currentState == AppState::AddingObstacle) {
        item->setText("Engel");
        item->setBackground(QColor(0, 0, 0));
        item->setForeground(QColor(255, 255, 255));
        m_matrixData[row][col] = 1; // Engel: 1
        return;
    }
    if (currentState == AppState::AddingMine) {
        if (item->text() == "Deniz" || item->text() == "X") {
            item->setText("X");
            item->setBackground(QColor(0, 191, 255));
            item->setForeground(QColor(255, 0, 0));
            m_matrixData[row][col] = 3; // Mayin: 3
        }
        else {
            QMessageBox::warning(this, "Uyari", "Mayin sadece denize yerlestirilebilir.");
        }
        return;
    }

    // ---

    // 2) Koordinat Secimi Modu
    if (currentState == AppState::SettingStart || currentState == AppState::SettingEnd) {
        QString cellText = item->text();
        FindPath::Vehicle currentVehicle = m_vehicles.at(m_vehicleIndex);

        // Secilen konumun arac tipine uygunluðunu kontrol et
        if (currentVehicle == FindPath::Vehicle::Land && cellText != "Kara") {
            QMessageBox::warning(this, "Uyari", "Kara araci sadece 'Kara' uzerine baslayabilir/bitebilir.");
            return;
        }
        else if (currentVehicle == FindPath::Vehicle::Sea && cellText != "Deniz" && cellText != "X") {
            QMessageBox::warning(this, "Uyari", "Deniz araci sadece 'Deniz' veya 'Mayin' uzerine baslayabilir/bitebilir.");
            return;
        }

        if (currentState == AppState::SettingStart) {
            // Baslangic noktasini kaydet ve bitis noktasi moduna gec
            m_startPoints[currentVehicle] = { row, col };
            infoLabel->setText(QString("Lutfen %1 araci icin bitis noktasini secin.").arg(vehicleToText(currentVehicle)));
            currentState = AppState::SettingEnd;
        }
        else if (currentState == AppState::SettingEnd) {
            // Bitis noktasini kaydet
            m_endPoints[currentVehicle] = { row, col };
            resultsTextEdit->append(QString("-> %1 araci icin baslangic: (%2, %3), Bitis: (%4, %5)").arg(vehicleToText(currentVehicle)).arg(m_startPoints[currentVehicle].r).arg(m_startPoints[currentVehicle].c).arg(m_endPoints[currentVehicle].r).arg(m_endPoints[currentVehicle].c));

            // Bir sonraki araca gecis yap
            m_vehicleIndex++;

            if (m_vehicleIndex < m_vehicles.size()) {
                // Hâlâ islenecek araclar var
                currentState = AppState::SettingStart;
                infoLabel->setText(QString("Lutfen %1 araci icin baslangic noktasini secin.").arg(vehicleToText(m_vehicles.at(m_vehicleIndex))));
            }
            else {
                // Tum araclarin koordinatlari belirlendi
                currentState = AppState::None;
                infoLabel->setText("Tum araclarin baslangic ve bitis noktalari belirlendi.");
                findPathButton->show(); // Artik "Yolu Bul" butonu gorunebilir.
                findPathButton->setEnabled(true);

                skipButton->hide();
                skipButton->setEnabled(false);
            }
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
void QtWidgetsApplication2::findPath()
{
    resultsTextEdit->clear();
    resultsTextEdit->append("--- Yol Bulma baslatildi ---");

    ThreadManager* manager = new ThreadManager(this);

    connect(manager, &ThreadManager::landFinished, this, [=](FindPath::PathResult res) {
        if (!res.nodes.empty())
        {
            // total adim sayisi
            int stepCount = res.nodes.size() - 1;

            // definitionstan hizi cek 
            Speed vehicleSpeeds;
            float landSpeed = vehicleSpeeds.land;

            //gecen zamaný goster 
            float timeInSeconds =  (stepCount) / landSpeed;

            // Display the results.
            resultsTextEdit->append(
                QString("Kara araci icin yol bulundu: Toplam %1 adim.").arg(stepCount));
            resultsTextEdit->append(
                QString("Yaklasik yol suresi: %1 saniye.").arg(timeInSeconds, 0, 'f', 2));

            // adimlari göster
            QString path;
            for (const auto& cell : res.nodes) {
                path += QString("(%1,%2) ").arg(cell.r).arg(cell.c);
            }
            resultsTextEdit->append("Yol: " + path);
        }
        else
        {
            resultsTextEdit->append("Kara araci aktive edilmedi.");
        }
        });

    connect(manager, &ThreadManager::seaFinished, this, [=](FindPath::PathResult res) {
        if (!res.nodes.empty())
        {
            
        }
        else
        {
            resultsTextEdit->append("Deniz araci aktive edilmedi.");
        }
        });

    connect(manager, &ThreadManager::airFinished, this, [=](FindPath::PathResult res) {
        if (!res.nodes.empty()) { // PathResult'ýn boþ olmadýðýný kontrol et
            Speed vehicleSpeeds;
            float timeInSeconds = res.distance / vehicleSpeeds.air;

            resultsTextEdit->append(
                QString("Hava araci icin yol bulundu: Toplam %1 birim mesafe. (Yaklasik %2 saniye)")
                .arg(res.distance, 0, 'f', 2) // Virgülden sonra 2 basamak göster
                .arg(timeInSeconds, 0, 'f', 2));

            // Hava aracýnýn yolu düz bir çizgi olduðu için sadece baþlangýç ve bitiþ noktalarýný göster.
            QString path;
            path += QString("(%1,%2) ").arg(res.nodes[0].r).arg(res.nodes[0].c);
            path += "--> ";
            path += QString("(%1,%2)").arg(res.nodes[1].r).arg(res.nodes[1].c);

            resultsTextEdit->append("Yol: " + path);
        }
        else {
            resultsTextEdit->append("Hava araci aktive edilmedi.");
        }
        });

    if (m_startPoints.contains(FindPath::Vehicle::Land))
        manager->runLand(m_matrixData,
            m_startPoints[FindPath::Vehicle::Land],
            m_endPoints[FindPath::Vehicle::Land]);

    if (m_startPoints.contains(FindPath::Vehicle::Sea))
        manager->runSea(m_matrixData,
            m_startPoints[FindPath::Vehicle::Sea],
            m_endPoints[FindPath::Vehicle::Sea]);

    if (m_startPoints.contains(FindPath::Vehicle::Air))
        manager->runAir(m_matrixData,
            m_startPoints[FindPath::Vehicle::Air],
            m_endPoints[FindPath::Vehicle::Air]);
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

    // QTableWidget'i okunan boyutlara gore ayarla
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
    // once matrisin olusturulduðunu kontrol et
    if (m_matrixData.empty() || m_matrixData[0].empty()) {
        QMessageBox::warning(this, "Hata", "Lutfen once bir matris olusturun veya yukleyin.");
        return;
    }

    // Arayuzu algoritma moduna gecir
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
	skipButton->show(); 

    infoLabel->show();
    resultsTextEdit->show();
    resetButton->show();
    findPathButton->hide(); // Baslangicta gizle
    

    // Ýslenecek arac listesini olustur ve indeksi sifirla
    m_vehicles = { FindPath::Vehicle::Land, FindPath::Vehicle::Sea, FindPath::Vehicle::Air };
    m_vehicleIndex = 0;

    // Uygulamanin durumunu baslangic noktasi secimine ayarla
    currentState = AppState::SettingStart;

    // Kullaniciya ilk araci icin baslangic noktasi secmesini soyle
    infoLabel->setText(QString("Lutfen **%1** araci icin baslangic noktasini secin.").arg(vehicleToText(m_vehicles.at(m_vehicleIndex))));
}

void QtWidgetsApplication2::setStartPoint()
{
    // once bitis secim modunu kapatalim
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
    // once baslangic secim modunu kapatalim
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


void QtWidgetsApplication2::resetUI()
{
	// 
}
QString QtWidgetsApplication2::vehicleToText(FindPath::Vehicle v) {
    switch (v) {
    case FindPath::Vehicle::Land: return "Kara";
    case FindPath::Vehicle::Sea:  return "Deniz";
    case FindPath::Vehicle::Air:  return "Hava";
    }
    return "?";
}

void QtWidgetsApplication2::skipVehicle()
{
    resultsTextEdit->append(QString("-> **%1** araci atlandi.").arg(vehicleToText(m_vehicles.at(m_vehicleIndex))));

    // Bir sonraki araca gecis yap
    m_vehicleIndex++;

    if (m_vehicleIndex < m_vehicles.size()) {
        // Hâlâ islenecek araclar var
        currentState = AppState::SettingStart;
        infoLabel->setText(QString("Lutfen **%1** araci icin baslangic noktasini secin.").arg(vehicleToText(m_vehicles.at(m_vehicleIndex))));
    }
    else {
        // Tum araclarin koordinatlari belirlendi veya atlandi
        currentState = AppState::None;
        infoLabel->setText("Tum araclarin baslangic ve bitis noktalari belirlendi veya atlandi.");
        findPathButton->show();
        findPathButton->setEnabled(true);
        skipButton->hide();
		skipButton->setEnabled(false);
    }
}

