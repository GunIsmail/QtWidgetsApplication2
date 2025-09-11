#ifndef QTWidgetsApplication2_H
#define QTWidgetsApplication2_H

#include <QtWidgets/QWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QColor>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <vector>
#include <QStringList>
#include <QTextEdit>
#include <QComboBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include "FindPath.h"
#include "ThreadManager/threadManager.h"
#include "definitions.h"
#include "visualization.h"
#include "VehicleManager/vehicle/vehicle.h"
#include "VehicleManager/land/land.h"
#include "VehicleManager/sea/sea.h"
#include "VehicleManager/air/air.h"

enum class AppState {
    None,
    AddingSea,
    AddingObstacle,
    AddingMine,
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

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    // UI
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
    QTableWidget* matrixTable;
    QComboBox* vehicleComboBox;

    // Data
    std::vector<std::vector<int>> m_matrixData;

    struct VehicleTask {
        Vehicle* vehicle;
        FindPath::Cell start;
        FindPath::Cell end;
    };

    QVector<VehicleTask> m_vehicleTasks;
    Vehicle* m_currentVehicle = nullptr;
    FindPath::Cell m_tempStart;
    AppState currentState;
    QString m_currentFilePath;

    // Helpers
    QString vehicleToText(Vehicle* v);
    void printAndVisualizeResult(const QString& vehicleName,
        const FindPath::PathResult& res,
        double speed,
        QColor color,
        int lineWidth,
        bool showMines = false);
};

#endif // QTWidgetsApplication2_H
