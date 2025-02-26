#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include <vector>

// Tile types for the grid.
enum TileType {
  EMPTY,
  PATH,
  ENTRY,
  EXIT,
  FREEZINGTOWER,
  SNIPERTOWER,
  BOMBTOWER,
  REGULARTOWER,
  WOLF,
  SQUIRREL,
  BEAR
};

class Map {
public:
  int gridWidth, gridHeight, tileSize;
  std::vector<std::vector<TileType>> grid;
  int entryRow, entryCol;
  int exitRow, exitCol;

  // Constructor: initializes grid dimensions and sets entry/exit as unset.
  Map(int width, int height, int tileSize);
  ~Map(); // Destructor, if needed

  // Draws the grid and instructions.
  void Draw();

  // Toggles a cell's state between empty and path.
  void TogglePath(int row, int col);

  // Sets the entry point.
  void SetEntry(int row, int col);

  // Sets the exit point.
  void SetExit(int row, int col);

  // Validates if there is a contiguous path from entry to exit.
  bool IsValidPath();

  // Runs the map editor interface.
  // Returns true if the user confirms a valid map.
  bool RunEditor();
};

void RunGame(Map &map);
#endif // MAP_H
