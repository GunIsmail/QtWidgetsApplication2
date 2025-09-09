#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QColor>
#include <QIcon>
// Araç hýzlarý
struct Speed {
    static constexpr double land = 1.0;
    static constexpr double sea = 1.0;
    static constexpr double air = 5.0;
};

// Görselleþtirme ayarlarý
namespace VisualizationConfig {
    constexpr int STEP_DELAY_MS = 500;   // animasyon gecikmesi (ms)
    constexpr int CELL_SIZE = 30;  // cell boyutu 
    // Renkler
    const QColor LAND_COLOR = QColor(0, 128, 0);   // yeþil
    const QColor SEA_COLOR = QColor(0, 0, 255);   // mavi
    const QColor AIR_COLOR = QColor(255, 0, 0);   // kýrmýzý
    const QColor SCAN_COLOR = QColor(199,21,133);   // medium violet purple 
    const QColor SEARCH_COLOR = QColor(255, 255, 0); // sarý

    const QColor OBSTACLE_COLOR = QColor(0, 0, 0);           // engel siyah
    const QColor OBSTACLE_TEXT_COLOR = QColor(0, 0, 0);

    const QColor LAND_BG_COLOR = QColor(139, 69, 19);         // kahverengi kara zemini
    const QColor LAND_TEXT_COLOR = QColor(139, 69, 19);

    const QColor SEA_TEXT_COLOR = QColor(0, 0, 255); 
    const QColor MINE_TEXT_COLOR = QColor(255, 0, 0);

    // Çizgi kalýnlýklarý
    constexpr int LAND_WIDTH = 3;
    constexpr int SEA_WIDTH = 3;
    constexpr int AIR_WIDTH = 2;

    const QIcon AIR_ICON = QIcon(":/icons/plane.png");
    const QIcon LAND_ICON = QIcon(":/icons/tank.png");
    const QIcon SEA_ICON = QIcon(":/icons/ship.png");
}

#endif // DEFINITIONS_H
