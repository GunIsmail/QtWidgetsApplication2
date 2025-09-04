#pragma once
#include <vector>

class FindPath {
public:
    struct Cell {
        int r, c;
        bool operator==(const Cell& o) const { return r == o.r && c == o.c; }
    };

    enum class Dir { Up, Down, Left, Right };

    struct PathResult {
        std::vector<Cell> nodes;  // start ve goal DAHÝL
        std::vector<Dir>  dirs;   // boyut = nodes.size() - 1 (her kenar için bir yön)

        int steps() const { return nodes.empty() ? 0 : (int)nodes.size() - 1; }
        bool empty() const { return nodes.empty(); }
    };

    using Grid = std::vector<std::vector<int>>;

    // Yönlerle birlikte tam sonuç
    static PathResult AStarWithDirs(const Grid& grid, Cell start, Cell goal);

    // Geriye dönük uyum: sadece düðümler (eski imza)
    static std::vector<Cell> AStar(const Grid& grid, Cell start, Cell goal);

    // Ýlk adýmý döndür (yoksa {-1,-1})
    static Cell NextStep(const Grid& grid, Cell start, Cell goal);

private:
    static bool inBounds(int r, int c, int R, int C);
    static bool passable(const Grid& grid, int r, int c);
    static int  manhattan(Cell a, Cell b);
};
