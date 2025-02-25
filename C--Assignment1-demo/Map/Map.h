#ifndef MAP_H
#define MAP_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>


class Observer;
class Map {
private:
  enum CellType { SCENERY, PATH, TOWER, CRITTER };
  std::vector<Observer*> observers;

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

  std::pair<int, int> getEntryPoint() { return entryPoint; }
  std::pair<int, int> getExitPoint() { return exitPoint; }

  int getHeight() { return height; }
  int getWidth() { return width; }

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

  // Observer management
  void attach(Observer* observer);
  void detach(Observer* observer);
  void notify();


};

#endif // MAP_H
