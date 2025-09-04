#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>
#include <QColor>

// Kaydedilecek dosya adýný tanýmlarýz.
extern const QString MATRIX_FILE_NAME;

// Matris hücrelerini temsil eden enum'lar
enum class MatrixType {
    Kara = 0,
    Engel = 1,
    Deniz = 2,
    Mayin=3
};


const char* getTypeString(MatrixType type);
QColor getTypeColor(MatrixType type);

#endif // DEFINITIONS_H