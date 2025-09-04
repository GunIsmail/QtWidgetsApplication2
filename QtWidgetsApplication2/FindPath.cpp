#include "FindPath.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>

struct Node {
    int r, c;
    int g; // maliyet
    int f; // g + h
};
struct ByF {
    bool operator()(const Node& a, const Node& b) const { return a.f > b.f; }
};

static inline FindPath::Dir dirFromDelta(int pr, int pc, int cr, int cc) {
    if (cr == pr - 1 && cc == pc)     return FindPath::Dir::Up;
    if (cr == pr + 1 && cc == pc)     return FindPath::Dir::Down;
    if (cr == pr && cc == pc - 1)     return FindPath::Dir::Left;
    /* cr == pr && cc == pc + 1 */    return FindPath::Dir::Right;
}

bool FindPath::inBounds(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}
bool FindPath::passable(const Grid& grid, int r, int c) {
    return inBounds(r, c, (int)grid.size(), (int)grid[0].size()) && grid[r][c] == 0; // sadece 0
}
int FindPath::manhattan(Cell a, Cell b) {
    return std::abs(a.r - b.r) + std::abs(a.c - b.c);
}

FindPath::PathResult
FindPath::AStarWithDirs(const Grid& grid, Cell start, Cell goal) {
    PathResult out;

    const int R = (int)grid.size();
    if (R == 0) return out;
    const int C = (int)grid[0].size();

    if (!passable(grid, start.r, start.c) || !passable(grid, goal.r, goal.c))
        return out;

    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0,-1, 1 };

    std::priority_queue<Node, std::vector<Node>, ByF> open;
    std::vector<std::vector<int>> g(R, std::vector<int>(C, std::numeric_limits<int>::max()));
    std::vector<std::vector<bool>> closed(R, std::vector<bool>(C, false));
    std::vector<std::vector<Cell>> parent(R, std::vector<Cell>(C, { -1, -1 }));

    g[start.r][start.c] = 0;
    open.push({ start.r, start.c, 0, manhattan(start, goal) });

    while (!open.empty()) {
        Node cur = open.top(); open.pop();
        if (closed[cur.r][cur.c]) continue;
        closed[cur.r][cur.c] = true;

        if (cur.r == goal.r && cur.c == goal.c) {
            // -- Yol geri çýkarma: nodes ve dirs birlikte --
            std::vector<Cell> revNodes;
            std::vector<Dir>  revDirs;

            Cell p = goal;
            while (!(p == start)) {
                Cell q = parent[p.r][p.c];  // p'nin ebeveyni
                revNodes.push_back(p);
                revDirs.push_back(dirFromDelta(q.r, q.c, p.r, p.c)); // q -> p yönü
                p = q;
            }
            revNodes.push_back(start); // en sonda startý ekle

            // Ters çevir
            std::reverse(revNodes.begin(), revNodes.end()); // start ... goal
            std::reverse(revDirs.begin(), revDirs.end());   // start->...->goal yönleri

            out.nodes = std::move(revNodes);
            out.dirs = std::move(revDirs);
            return out;
        }

        for (int k = 0; k < 4; ++k) {
            int nr = cur.r + dr[k], nc = cur.c + dc[k];
            if (!passable(grid, nr, nc) || closed[nr][nc]) continue;
            int tentative_g = g[cur.r][cur.c] + 1;
            if (tentative_g < g[nr][nc]) {
                g[nr][nc] = tentative_g;
                parent[nr][nc] = { cur.r, cur.c };
                int f = tentative_g + manhattan({ nr, nc }, goal);
                open.push({ nr, nc, tentative_g, f });
            }
        }
    }
    return out; // boþ -> yol yok
}

std::vector<FindPath::Cell>
FindPath::AStar(const Grid& grid, Cell start, Cell goal) {
    auto res = AStarWithDirs(grid, start, goal);
    return res.nodes;
}

FindPath::Cell FindPath::NextStep(const Grid& grid, Cell start, Cell goal) {
    auto res = AStarWithDirs(grid, start, goal);
    if (res.nodes.size() >= 2) return res.nodes[1];
    return { -1, -1 };
}
