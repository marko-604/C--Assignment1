#ifndef MAP_H
#define MAP_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

class Map {
private:
  enum CellType { SCENERY, PATH, OCCUPIED, CRITTER_OCCUPIED };

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

  bool bfsFindPath();

public:
  Map(int w, int h);

  bool isValidCoordinate(int x, int y);
  bool isPathCell(int x, int y);
  bool isCloserToExit(int x0, int x1, int y0, int y1);
  void setCellToPath(int x, int y);
  void setCellToScenery(int x, int y);
  void setCellToOccupied(int x, int y);
  void setEntryPoint(int x, int y);
  void setExitPoint(int x, int y);
  void setCellToCritterCell(int x, int y);

  bool isSceneryCell(int x_val, int y_val);
  bool validateMap();
  void displayMap();
  void displayEntityPath();
};

#endif // MAP_H
