#ifndef QTWidgetsApplication2_H
#define QTWidgetsApplication2_H

#include <QtWidgets/QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <Qlist>
#include <QMap>
#include <Qvector>
#include "FindAlgorithmWindow.h"
#include "FindPath.h"
#include "ui_QtWidgetsApplication2.h" 
#include "VehicleSelectionDialog.h"
#include "ThreadManager/threadManager.h"
#include "definitions.h"
#include "visualization.h"

#include <QtWidgets/QMainWindow> 



enum class AppState {
    None,
    AddingSea,
    AddingObstacle,
    AddingMine ,
    SettingStart,   
    SettingEnd     
};

class QtWidgetsApplication2 : public QWidget
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget* parent = nullptr);
    ~QtWidgetsApplication2();

private slots:
    void createMatrix();
    void addObstacleClicked();
    void addSeaClicked();
    void addMineClicked(); 
    void cellClicked(int row, int col);
    void saveMatrix();
    void loadMatrix();
    void findAlgorithm();
    void setStartPoint();
    void setEndPoint();
    void findPath();
    void resetUI();
    void skipVehicle();
    void addVehicle();
    void resizeEvent(QResizeEvent* event);
    void  keyPressEvent(QKeyEvent* event);



private:
    Ui::QtWidgetsApplication2Class ui;

    QLineEdit* nLineEdit;
    QLineEdit* mLineEdit;
    QPushButton* createButton;
    QPushButton* loadButton;
    QPushButton* addObstacleButton;
    QPushButton* addSeaButton;
    QPushButton* addMineButton; 
    QPushButton* saveButton;
    QPushButton* findAlgorithmButton;
    QPushButton* addVehicleButton;
    QPushButton* setStartButton;
    QPushButton* setEndButton;
    QPushButton* findPathButton;
    QPushButton* resetButton;
    QPushButton* skipButton;
    QLabel* nLabel;
    QLabel* mLabel;
    QLabel* infoLabel;
    QTextEdit* resultsTextEdit;
	QTableWidgetItem* startPointItem = nullptr;
    QTableWidgetItem* endPointItem = nullptr;
    QTableWidget* matrixTable;
    QComboBox* vehicleComboBox;


    std::vector<std::vector<int>> m_matrixData;

    // Baþlangýç / Bitiþ koordinatlarý
    int startRow = -1;
    int startCol = -1;
    int endRow = -1;
    int endCol = -1;
    enum class Vehicle { Land, Sea, Air };

    QMap<FindPath::Vehicle, FindPath::Cell> m_startPoints;
    QMap<FindPath::Vehicle, FindPath::Cell> m_endPoints;

    // Ýþlenecek araç listesi ve mevcut aracýn indeksi
    QList<FindPath::Vehicle> m_vehicles;
    int m_vehicleIndex = 0;

    FindPath::Vehicle m_currentVehicle;
    // Yardýmcý fonksiyon
    QString vehicleToText(FindPath::Vehicle v);

    struct VehicleTask {
        FindPath::Vehicle type;
        FindPath::Cell start;
        FindPath::Cell end;
    };

    // Eklenecek degiskenler
    QVector<VehicleTask> m_vehicleTasks;
    FindPath::Cell m_tempStart;

    AppState currentState;
    QString m_currentFilePath;

    void printAndVisualizeResult(const QString& vehicleName,
        const FindPath::PathResult& res,
        double speed,
        QColor color,
        int lineWidth,
        bool showMines = false);

};

#endif // QTWidgetsApplication2_H