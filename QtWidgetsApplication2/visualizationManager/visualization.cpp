#include "visualization.h"
#include <QTableWidgetItem>
#include <QThread>
#include <QCoreApplication>

Visualization::Visualization(QTableWidget* table)
    : m_table(table) {
}

void Visualization::visualizeStep(const FindPath::Cell& cell, QColor color) {
    if (!m_table) return;

    QTableWidgetItem* item = m_table->item(cell.r, cell.c);
    if (!item) return;

    item->setBackground(color);
    item->setForeground(Qt::white);

    QCoreApplication::processEvents(); // anýnda UI güncelle
}

void Visualization::visualizePath(const std::vector<FindPath::Cell>& path,
    QColor color,
    int delayMs) {
    for (const auto& step : path) {
        visualizeStep(step, color);
        QThread::msleep(delayMs); // adým adým animasyon
    }
}
QTableWidget* Visualization::table() const 
{
    return m_table; 
}