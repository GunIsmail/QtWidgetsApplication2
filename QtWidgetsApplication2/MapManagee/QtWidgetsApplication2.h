#ifndef QTWidgetsApplication2_H
#define QTWidgetsApplication2_H

#include <QtWidgets/QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

// Uygulama durumlarýný tanýmlayan bir enum
enum class AppState {
    None,
    AddingSea,
    AddingObstacle,
    AddingMine 
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

private:
    QLineEdit* nLineEdit;
    QLineEdit* mLineEdit;
    QPushButton* createButton;
    QPushButton* loadButton;
    QPushButton* addObstacleButton;
    QPushButton* addSeaButton;
    QPushButton* addMineButton; 
    QPushButton* saveButton;
    QTableWidget* matrixTable;
    QLabel* nLabel;
    QLabel* mLabel;

    AppState currentState;
    QString m_currentFilePath;
};

#endif // QTWidgetsApplication2_H