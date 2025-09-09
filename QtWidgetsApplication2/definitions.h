#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QColor>

// Ara� h�zlar�
struct Speed {
    double land = 1.0;
    double sea = 1.0;
    double air = 1.0;
};

// G�rselle�tirme ayarlar�
namespace VisualizationConfig {
    constexpr int STEP_DELAY_MS = 500;   // animasyon gecikmesi (ms)
    constexpr int CELL_SIZE = 30;  // cell boyutu 
    // Renkler
    const QColor LAND_COLOR = QColor(0, 128, 0);   // ye�il
    const QColor SEA_COLOR = QColor(0, 0, 255);   // mavi
    const QColor AIR_COLOR = QColor(255, 0, 0);   // k�rm�z�
    const QColor SEARCH_COLOR = QColor(255, 255, 0); // sar�

    // �izgi kal�nl�klar�
    constexpr int LAND_WIDTH = 3;
    constexpr int SEA_WIDTH = 3;
    constexpr int AIR_WIDTH = 2;
}

#endif // DEFINITIONS_H
