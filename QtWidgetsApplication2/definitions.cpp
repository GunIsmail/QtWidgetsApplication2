#include "definitions.h"
#include <QColor>

// Kaydedilecek dosya ad�n�n tan�m�
const QString MATRIX_FILE_NAME = "matrix.txt";

const char* getTypeString(MatrixType type) {
    switch (type) {
    case MatrixType::Kara: return "Kara";
    case MatrixType::Engel: return "Engel";
    case MatrixType::Deniz: return "Deniz";
    }
    return ""; //defautl deger 
}

QColor getTypeColor(MatrixType type) {
    switch (type) {
    case MatrixType::Kara: return QColor(139, 69, 19);
    case MatrixType::Engel: return QColor(0, 0, 0);
    case MatrixType::Deniz: return QColor(0, 191, 255);
    case MatrixType::Mayin: return QColor(255, 0, 0)
        ;
    }
    return QColor(); // Varsay�lan de�er
}