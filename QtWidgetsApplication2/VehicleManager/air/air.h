#ifndef AIR_H
#define AIR_H

#include "../FindPath.h" // Ortak yapýlar ve sýnýflar için
#include "../definitions.h" // Hýz bilgileri için

namespace Air {
    // Hava aracý için en basit yol bulma fonksiyonu
    FindPath::PathResult findPath(const FindPath::Grid& grid, FindPath::Cell start, FindPath::Cell goal);
}

#endif // AIR_H