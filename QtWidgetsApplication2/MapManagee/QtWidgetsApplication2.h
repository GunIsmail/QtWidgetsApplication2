#ifndef QTWidgetsApplication2_H
#define QTWidgetsApplication2_H

#include <QtWidgets/QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "FindAlgorithmWindow.h"
#include "FindPath.h"

// Uygulama durumlarýný tanýmlayan bir enum
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

private:

    QLineEdit* nLineEdit;
    QLineEdit* mLineEdit;
    QPushButton* createButton;
    QPushButton* loadButton;
    QPushButton* addObstacleButton;
    QPushButton* addSeaButton;
    QPushButton* addMineButton; 
    QPushButton* saveButton;
    QPushButton* findAlgorithmButton;
    QTableWidget* matrixTable;
    QLabel* nLabel;
    QLabel* mLabel;
    QPushButton* setStartButton;
    QPushButton* setEndButton;
    QPushButton* findPathButton;
    QPushButton* resetButton;
    QTextEdit* resultsTextEdit;
    QLabel* infoLabel;
	QTableWidgetItem* startPointItem = nullptr;
    QTableWidgetItem* endPointItem = nullptr;


    std::vector<std::vector<int>> m_matrixData;

    // Baþlangýç / Bitiþ koordinatlarý
    int startRow = -1;
    int startCol = -1;
    int endRow = -1;
    int endCol = -1;


    AppState currentState;
    QString m_currentFilePath;
   
};

#endif // QTWidgetsApplication2_H