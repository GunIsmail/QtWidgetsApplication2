#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "FindPath.h"
#include <QTableWidget>
#include <QColor>
#include <vector>

class Visualization {
public:
    explicit Visualization(QTableWidget* table);

    // Tek bir h�creyi renklendir
    void visualizeStep(const FindPath::Cell& cell, QColor color);
    QTableWidget* table() const;  

    // Yol boyunca ad�m ad�m h�creleri renklendir
    void visualizePath(const std::vector<FindPath::Cell>& path,
        QColor color,
        int delayMs);

private:
    QTableWidget* m_table;
};

#endif // VISUALIZATION_H
