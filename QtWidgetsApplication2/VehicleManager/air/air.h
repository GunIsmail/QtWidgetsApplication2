#ifndef AIR_H
#define AIR_H

#include "../FindPath.h" // Ortak yap�lar ve s�n�flar i�in
#include "../definitions.h" // H�z bilgileri i�in

namespace Air {
    // Hava arac� i�in en basit yol bulma fonksiyonu
    FindPath::PathResult findPath(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal);
}

#endif // AIR_H