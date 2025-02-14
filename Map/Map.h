#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>

class Map {
private:
    enum CellType { SCENERY, PATH };

    struct Cell {
        CellType type;
        bool isEntry = false;
        bool isExit = false;
    };

    int width, height;
    std::vector<std::vector<Cell>> grid;
    std::pair<int, int> entryPoint = {-1, -1};
    std::pair<int, int> exitPoint = {-1, -1};

    std::vector<std::pair<int, int>> entityPath;

    bool isValidCoordinate(int x, int y);
    bool bfsFindPath();

public:
    Map(int w, int h);

    void setCellToPath(int x, int y);
    void setCellToScenery(int x, int y);
    void setEntryPoint(int x, int y);
    void setExitPoint(int x, int y);

    bool validateMap();
    void displayMap();
    void displayEntityPath();
};

#endif // MAP_H
