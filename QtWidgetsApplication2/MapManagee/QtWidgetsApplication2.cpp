#include "QtWidgetsApplication2.h"


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
    addEnemyButton = new QPushButton("Düşman Ekle", this);

    saveButton = new QPushButton("Kaydet", this);
    matrixTable = new QTableWidget(this);
    findAlgorithmButton = new QPushButton("Algoritma Modu", this);

    // Algoritma modu icin yeni bilesenler
    setStartButton = new QPushButton("Baslangic", this);
    setEndButton = new QPushButton("Bitis", this);
    findPathButton = new QPushButton("Yolu Bul", this);
    resetButton = new QPushButton("Geri Don", this);
    resultsTextEdit = new QTextEdit(this);
    infoLabel = new QLabel("Arac ekleyin.", this);
    skipButton = new QPushButton("Atla", this);

    // Arac ekleme mekanizmasi
    addVehicleButton = new QPushButton("Arac Ekle", this);
    vehicleComboBox = new QComboBox(this);
    vehicleComboBox->addItem("Kara");
    vehicleComboBox->addItem("Deniz");
    vehicleComboBox->addItem("Hava");

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

    // Layoutlar
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
    


    QHBoxLayout* vehicleLayout = new QHBoxLayout();
    vehicleLayout->addWidget(vehicleComboBox);
    vehicleLayout->addWidget(addVehicleButton);
    vehicleLayout->addWidget(addEnemyButton);

    QHBoxLayout* algorithmButtonsLayout = new QHBoxLayout();
    algorithmButtonsLayout->addWidget(setStartButton);
    algorithmButtonsLayout->addWidget(setEndButton);
    algorithmButtonsLayout->addWidget(findPathButton);
    algorithmButtonsLayout->addWidget(skipButton);

    QVBoxLayout* algorithmLayout = new QVBoxLayout();
    algorithmLayout->addWidget(infoLabel);
    algorithmLayout->addLayout(vehicleLayout);
    algorithmLayout->addLayout(algorithmButtonsLayout);
    algorithmLayout->addWidget(resultsTextEdit);
    algorithmLayout->addWidget(resetButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(matrixTable);
    mainLayout->addLayout(actionButtonLayout);
    mainLayout->addLayout(algorithmLayout, 0);

    // Baslangicta gizle
    infoLabel->hide();
    setStartButton->hide();
    setEndButton->hide();
    findPathButton->hide();
    resultsTextEdit->hide();
    resetButton->hide();
    skipButton->hide();
    addVehicleButton->hide();
    vehicleComboBox->hide();

    setLayout(mainLayout);
    setWindowTitle("Matrix Uygulamasi");

    // Sinyal-slot baglantilari
    connect(createButton, &QPushButton::clicked, this, &QtWidgetsApplication2::createMatrix);
    connect(loadButton, &QPushButton::clicked, this, &QtWidgetsApplication2::loadMatrix);
    connect(addObstacleButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addObstacleClicked);
    connect(addSeaButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addSeaClicked);
    connect(addMineButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addMineClicked);
    connect(matrixTable, &QTableWidget::cellClicked, this, &QtWidgetsApplication2::cellClicked);
    connect(saveButton, &QPushButton::clicked, this, &QtWidgetsApplication2::saveMatrix);
    connect(findAlgorithmButton, &QPushButton::clicked, this, &QtWidgetsApplication2::findAlgorithm);

    connect(setStartButton, &QPushButton::clicked, this, &QtWidgetsApplication2::setStartPoint);
    connect(setEndButton, &QPushButton::clicked, this, &QtWidgetsApplication2::setEndPoint);
    connect(findPathButton, &QPushButton::clicked, this, &QtWidgetsApplication2::findPath);
    connect(resetButton, &QPushButton::clicked, this, &QtWidgetsApplication2::resetUI);
    connect(skipButton, &QPushButton::clicked, this, &QtWidgetsApplication2::skipVehicle);
    connect(addEnemyButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addEnemies);


    connect(addVehicleButton, &QPushButton::clicked, this, &QtWidgetsApplication2::addVehicle);
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
        QMessageBox::warning(this, "Hata", "Lütfen geçerli pozitif tam sayılar girin.");
        return;
    }

    matrixTable->setRowCount(n);
    matrixTable->setColumnCount(m);

    int cellSize = VisualizationConfig::CELL_SIZE;   

    for (int i = 0; i < n; ++i)
        matrixTable->setRowHeight(i, cellSize);

    for (int j = 0; j < m; ++j)
        matrixTable->setColumnWidth(j, cellSize);

    // Hücreleri doldur
    m_matrixData.clear();
    m_matrixData.resize(n, std::vector<int>(m, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem("Kara");
            item->setBackground(QColor(139, 69, 19));
            item->setForeground(QColor(0, 0, 0));
            matrixTable->setItem(i, j, item);
            m_matrixData[i][j] = 0;
        }
    }

    matrixTable->horizontalHeader()->hide();
    matrixTable->verticalHeader()->hide();

    int tableWidth = m * cellSize + matrixTable->verticalHeader()->width();
    int tableHeight = n * cellSize + matrixTable->horizontalHeader()->height();
    this->resize(tableWidth + 50, tableHeight + 150);

    addObstacleButton->setEnabled(true);
    addSeaButton->setEnabled(true);
    addMineButton->setEnabled(true);
    saveButton->setEnabled(true);
    findAlgorithmButton->setEnabled(true);
}
void QtWidgetsApplication2::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (!matrixTable) return;

    int rows = matrixTable->rowCount();
    int cols = matrixTable->columnCount();
    if (rows == 0 || cols == 0) return;

    // Tablo görünür alanını al
    int w = matrixTable->viewport()->width();
    int h = matrixTable->viewport()->height();

    // Kare boyutu: hem genişlik hem yükseklik için min değer
    int cellSize = std::min(w / cols, h / rows);

    for (int r = 0; r < rows; ++r) {
        matrixTable->setRowHeight(r, cellSize);
    }
    for (int c = 0; c < cols; ++c) {
        matrixTable->setColumnWidth(c, cellSize);
    }
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

    // Edit mode
    if (currentState == AppState::AddingSea) {
        item->setText("Deniz");
        item->setBackground(VisualizationConfig::SEA_COLOR);
        item->setForeground(VisualizationConfig::SEA_TEXT_COLOR);
        m_matrixData[row][col] = 2;
        return;
    }
    if (currentState == AppState::AddingObstacle) {
        item->setText("Engel");
        item->setBackground(VisualizationConfig::OBSTACLE_COLOR);
        item->setForeground(VisualizationConfig::OBSTACLE_TEXT_COLOR);
        m_matrixData[row][col] = 1;
        return;
    }
    if (currentState == AppState::AddingMine) {
        if (item->text() == "Deniz" || item->text() == "X") {
            item->setText("X");
            item->setBackground(VisualizationConfig::SEA_COLOR);
            item->setForeground(VisualizationConfig::MINE_TEXT_COLOR);
            m_matrixData[row][col] = 3;
        }
        else {
            QMessageBox::warning(this, "Warning", "Mayını denize yerleştirin.");
        }
        return;
    }

    // Start/End selection
    if (currentState == AppState::SettingStart || currentState == AppState::SettingEnd) {
        QString cellText = item->text();

        if (dynamic_cast<LandVehicle*>(m_currentVehicle) && cellText != "Kara") {
            QMessageBox::warning(this, "Warning", "Kara aracı sadece kara parçasına konabilir.");
            return;
        }
        else if (dynamic_cast<SeaVehicle*>(m_currentVehicle) &&
            (cellText != "Deniz" && cellText != "X")) {
            QMessageBox::warning(this, "Warning", "Deniz aracı sadece denize veya mayına konabilir.");
            return;
        }

        if (currentState == AppState::SettingStart) {
            m_tempStart = { row, col };
            infoLabel->setText(QString("%1 aracı: bitiş noktası seç")
                .arg(vehicleToText(m_currentVehicle)));
            currentState = AppState::SettingEnd;
        }
        else if (currentState == AppState::SettingEnd) {
            VehicleTask task;
            task.vehicle = m_currentVehicle;
            task.start = m_tempStart;
            task.end = { row, col };

            m_vehicleTasks.append(task);

            resultsTextEdit->append(QString("-> %1: başlangıç (%2,%3), bitiş (%4,%5)")
                .arg(vehicleToText(m_currentVehicle))
                .arg(task.start.r).arg(task.start.c)
                .arg(task.end.r).arg(task.end.c));

            currentState = AppState::None;
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
    this->showFullScreen();

    matrixTable->show();
    infoLabel->show();
    resetButton->show();

    resultsTextEdit->clear();
    resultsTextEdit->append("--- Yol bulma başlatıldı ---");
    infoLabel->clear();

    ThreadManager* manager = new ThreadManager(this);

    connect(manager, &ThreadManager::vehicleFinished, this,
        [=](QString vehicleName, FindPath::PathResult res) {
            Speed speeds;
            if (vehicleName == "Kara") {
                printAndVisualizeResult(vehicleName, res, speeds.land,
                    VisualizationConfig::LAND_COLOR,
                    VisualizationConfig::LAND_WIDTH);
            }
            else if (vehicleName == "Deniz") {
                printAndVisualizeResult(vehicleName, res, speeds.sea,
                    VisualizationConfig::SEA_COLOR,
                    VisualizationConfig::SEA_WIDTH,
                    true);
            }
            else if (vehicleName == "Hava") {
                printAndVisualizeResult(vehicleName, res, speeds.air,
                    VisualizationConfig::AIR_COLOR,
                    VisualizationConfig::AIR_WIDTH);
            }
        });

    for (const auto& task : m_vehicleTasks) {
        Vehicle* v = task.vehicle;
        double speed = 1.0;

        if (dynamic_cast<LandVehicle*>(v)) speed = Speed::land;
        else if (dynamic_cast<SeaVehicle*>(v)) speed = Speed::sea;
        else if (dynamic_cast<AirVehicle*>(v)) speed = Speed::air;

        if (v) {
            manager->runVehicle(v, m_matrixData, task.start, task.end, matrixTable, speed);
        }
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

    // Boyutlarin gecerliliğini kontrol et
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

            if (value == 0) 
            {
                item->setText("Kara");
                item->setBackground(VisualizationConfig::LAND_BG_COLOR);
                item->setForeground((VisualizationConfig::LAND_TEXT_COLOR));
            }
            else if (value == 1) 
            {
                item->setText("Engel");
                item->setBackground(VisualizationConfig::OBSTACLE_COLOR);
                item->setForeground((VisualizationConfig::OBSTACLE_TEXT_COLOR));
            }
            else if (value == 2) 
            {
                item->setText("Deniz");
                item->setBackground(VisualizationConfig::SEA_COLOR);        
                item->setForeground((VisualizationConfig::SEA_TEXT_COLOR));
            }
            else if (value == 3) 
            {
                item->setText("X");
                item->setBackground(VisualizationConfig::SEA_COLOR);
                item->setForeground((VisualizationConfig::MINE_TEXT_COLOR));
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

    infoLabel->show();
    resultsTextEdit->show();
    resetButton->show();
    addVehicleButton->show();
    vehicleComboBox->show();

    findPathButton->show();
    findPathButton->setEnabled(true);

    infoLabel->setText("Arac ekleyin ve baslangic/bitis noktasi secin.");
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
QString QtWidgetsApplication2::vehicleToText(Vehicle* v) {
    if (!v) return "?";
    return v->name();   
}
void QtWidgetsApplication2::skipVehicle()
{
    resultsTextEdit->append(QString("-> %1 araci atlandi.")
        .arg(vehicleToText(m_currentVehicle)));

    currentState = AppState::None;
    infoLabel->setText("baska arac ekleyebilirsiniz.");
}
void QtWidgetsApplication2::addVehicle()
{
    QString selected = vehicleComboBox->currentText();

    if (selected == "Kara") {
        m_currentVehicle = new LandVehicle();
    }
    else if (selected == "Deniz") {
        m_currentVehicle = new SeaVehicle();
    }
    else if (selected == "Hava") {
        m_currentVehicle = new AirVehicle();
    }

    infoLabel->setText(QString("%1 aracı için başlangıç noktası seçin.")
        .arg(vehicleToText(m_currentVehicle)));
    currentState = AppState::SettingStart;
}
void QtWidgetsApplication2::printAndVisualizeResult(const QString& vehicleName, const FindPath::PathResult& res, double speed, QColor color, int lineWidth,bool showMines)
{
    if (!res.nodes.empty()) {
        float timeInSeconds = res.distance / speed;
        QMessageBox::information(this,
            "Sonuç",
            QString("%1 aracı tamamladı!\n\nAdım: %2\nSüre: %3 saniye")
            .arg(vehicleName)
            .arg(res.distance)
            .arg(timeInSeconds, 0, 'f', 2));

     
        QString path;
        for (const auto& cell : res.nodes)
            path += QString("(%1,%2) ").arg(cell.r).arg(cell.c);
        resultsTextEdit->append("yol: " + path);

        if (showMines && !res.mines.empty()) {
            resultsTextEdit->append("Bulunan mayinlar:");
            for (const auto& mine : res.mines)
                resultsTextEdit->append(QString("-> (%1,%2)").arg(mine.r).arg(mine.c));
        }

       
    }
    else {
        resultsTextEdit->append(QString("%1 araci icin yol bulunamadi.").arg(vehicleName));
    }
}
void QtWidgetsApplication2::resetUI()
{
    this->showNormal();
    // 1) State ve geçici verileri temizle
    currentState = AppState::None;
    m_vehicleTasks.clear();
    m_tempStart = { -1, -1 };
    infoLabel->clear();

    // 2) Algoritma modundaki buton ve alanları gizle
    infoLabel->hide();
    setStartButton->hide();
    setEndButton->hide();
    findPathButton->hide();
    resultsTextEdit->hide();
    resetButton->hide();
    skipButton->hide();
    addVehicleButton->hide();
    vehicleComboBox->hide();

    // 3) Normal edit modundaki butonları geri getir
    nLabel->show();
    mLabel->show();
    nLineEdit->show();
    mLineEdit->show();
    createButton->show();
    loadButton->show();
    addObstacleButton->show();
    addSeaButton->show();
    addMineButton->show();
    saveButton->show();
    findAlgorithmButton->show();

    // 4) m_matrixData'dan tabloyu yeniden çiz
    if (!m_matrixData.empty()) {
        int rows = m_matrixData.size();
        int cols = m_matrixData[0].size();
        matrixTable->setRowCount(rows);
        matrixTable->setColumnCount(cols);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int value = m_matrixData[i][j];
                QTableWidgetItem* item = new QTableWidgetItem();
                if (value == 0) { // Kara
                    item->setText("Kara");
                    item->setBackground(VisualizationConfig::LAND_BG_COLOR);
                    item->setForeground(VisualizationConfig::LAND_TEXT_COLOR);
                }
                else if (value == 1) { // Engel
                    item->setText("Engel");
                    item->setBackground(VisualizationConfig::OBSTACLE_COLOR);
                    item->setForeground(VisualizationConfig::OBSTACLE_TEXT_COLOR);
                }
                else if (value == 2) { // Deniz
                    item->setText("Deniz");
                    item->setBackground(VisualizationConfig::SEA_COLOR);
                    item->setForeground(VisualizationConfig::SEA_TEXT_COLOR);
                }
                else if (value == 3) { // Mayın
                    item->setText("X");
                    item->setBackground(VisualizationConfig::SEA_COLOR);
                    item->setForeground(VisualizationConfig::MINE_TEXT_COLOR);
                }

                matrixTable->setItem(i, j, item);
            }
        }
    }


    resultsTextEdit->clear();

    QMessageBox::information(this, "Reset", "harita resetlendi.");
}
void QtWidgetsApplication2::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        resetUI();
    }

    QWidget::keyPressEvent(event);
}
void QtWidgetsApplication2::addEnemies()
{
    if (m_matrixData.empty()) {
        QMessageBox::warning(this, "Hata", "Önce bir harita oluşturun.");
        return;
    }

    bool ok;
    int count = QInputDialog::getInt(this, "Düsman Sayisi",
        "Kac düsman eklemek istiyorsunuz?",
        5, 1, 50, 1, &ok);
    if (!ok) return;

    if (!m_enemyManager) {
        m_enemyManager = new EnemyManager(m_matrixData);
    }

    // Random yerleştirme
    int rows = m_matrixData.size();
    int cols = m_matrixData[0].size();

    int placed = 0;
    while (placed < count) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (m_matrixData[r][c] == 0 && !m_enemyManager->isOccupied(r, c)) {
            m_enemyManager->addEnemy({ r, c });
            placed++;
        }
    }

    
    if (!m_enemyThread) {
        Visualization* viz = new Visualization(matrixTable);
        m_enemyThread = new EnemyThread(m_enemyManager, viz, Speed::enemy);
        m_enemyThread->start();
    }

    resultsTextEdit->append(QString("%1 düsman haritaya eklendi.").arg(count));
}
