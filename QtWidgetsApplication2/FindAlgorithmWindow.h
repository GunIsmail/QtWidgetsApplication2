#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <vector>

class FindAlgorithmWindow : public QDialog
{
    Q_OBJECT

public:
    FindAlgorithmWindow(const std::vector<std::vector<int>>& matrixData, QWidget* parent = nullptr);
    ~FindAlgorithmWindow();

private slots:
    void cellClicked(int row, int col);
    void setStartPoint();
    void setEndPoint();
    void findPath();

private:
    QTableWidget* matrixTable;
    QPushButton* setStartButton;
    QPushButton* setEndButton;
    QPushButton* findPathButton;
    QTextEdit* resultsTextEdit;
    QLabel* infoLabel;

    QTableWidgetItem* startPointItem = nullptr;
    QTableWidgetItem* endPointItem = nullptr;

    enum class AppState {
        None,
        SettingStart,
        SettingEnd
    };
    AppState currentState;
    std::vector<std::vector<int>> m_matrixData;

    void updateTableFromMatrix();
};