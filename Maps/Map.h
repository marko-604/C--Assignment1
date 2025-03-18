#ifndef MAP_H
#define MAP_H

#include "../Observer/Subject.h"
#include "raylib.h"
#include <utility>
#include <vector>

class Critter;
class Tower;
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
  WOLFCRITTER,
  SQUIRRELCRITTER,
  BEARCRITTER
};

class Map : public Subject {
public:
  int gridWidth, gridHeight, tileSize;
  std::vector<std::vector<TileType>> grid;
  int entryRow, entryCol;
  int exitRow, exitCol;

  // This method will return a path that will be from the entry point to the
  // exit point.
  std::vector<std::pair<int, int>> getPath();

  // Constructor: initializes grid dimensions and sets entry/exit as unset.
  Map(int width, int height, int tileSize);
  ~Map(); // Destructor, if needed

  // Draws the grid and instructions.
  void Draw();

  void setToPath(int row, int col);

  void ToggleTower(Tower *tower, int row, int col);
  void ToggleCritter(Critter *critter, int row, int col);

  // Places a critter on the map
  bool placeCritter(Critter &critter, int row, int col);

  // Places a tower on the map.
  bool PlaceTower(Tower &tower, int row, int col);

  // Toggles a cell's state between empty and path.
  void TogglePath(int row, int col);

  // Sets the entry point.
  void SetEntry(int row, int col);

  // Sets the exit point.
  void SetExit(int row, int col);

  // This method will make it so that the cell coordiantes change from anything
  // else to secnery.
  void setToScenery(int row, int col);

  // Validates if there is a contiguous path from entry to exit.
  bool IsValidPath();

  // Runs the map editor interface.
  // Returns true if the user confirms a valid map.
  bool RunEditor();
};

// This function will run the actual game later on we will place this function
// in the game master file for the implementation.
void RunGame(Map &map);
#endif // MAP_H
