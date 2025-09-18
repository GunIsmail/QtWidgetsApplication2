#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QColor>
#include <QIcon>
// Ara� h�zlar� statik nesne baslatmak �stemed�m cagirirken. Bunlar i�in her nesneye ayr� kopya a�maya gerek yok. Bellekten yer kazan�yoruz. 
struct Speed {
    static constexpr double land = 3.0;
    static constexpr double sea = 1.0;
    static constexpr double air = 5.0;
    static constexpr double enemy = 1.0;
    static constexpr int refreshSpeed = 100; // ms cinsinden deger girin 
};


// G�rselle�tirme ayarlar�
namespace VisualizationConfig {
    constexpr int STEP_DELAY_MS = 1000;   // animasyon gecikmesi (ms)
    constexpr int CELL_SIZE = 50;  // cell boyutu 
    // Renkler
    const QColor LAND_COLOR = QColor(0, 128, 0);   // ye�il
    const QColor SEA_COLOR = QColor(0, 0, 255);   // mavi
    const QColor AIR_COLOR = QColor(255, 0, 0);   // k�rm�z�
    const QColor SCAN_COLOR = QColor(0,245,255);   //turkuaz
    const QColor SEARCH_COLOR = QColor(255, 255, 0); // sar�
    const QColor MINE_COLOR = QColor(139, 0, 0); // mayin kirmizisi

    const QColor OBSTACLE_COLOR = QColor(0, 0, 0);          // engel siyah
    const QColor OBSTACLE_TEXT_COLOR = QColor(0, 0, 0);

    const QColor LAND_BG_COLOR = QColor(139, 69, 19);         // kahverengi kara zemini
    const QColor LAND_TEXT_COLOR = QColor(139, 69, 19);

    const QColor SEA_TEXT_COLOR = QColor(0, 0, 255);
    const QColor MINE_TEXT_COLOR = QColor(255, 0, 0);

    // �izgi kal�nl�klar�
    constexpr int LAND_WIDTH = 3;
    constexpr int SEA_WIDTH = 3;
    constexpr int AIR_WIDTH = 2;

    inline QIcon airIcon() { return QIcon("icons/plane.png"); }
    inline QIcon landIcon() { return QIcon("icons/tank.png"); }
    inline QIcon seaIcon() { return QIcon("icons/ship.png"); }
    inline QIcon mineIcon() { return QIcon("icons/mine.png"); }
    inline QIcon enemyIcon() { return QIcon("icons/enemy.png"); }
	inline QIcon startIcon() { return QIcon("icons/start.png"); }
	inline QIcon endIcon() { return QIcon("icons/end.png"); }


}
namespace noiseRate {
    const  int seaNoiceRate = 50;
}
#endif // DEFINITIONS_H
