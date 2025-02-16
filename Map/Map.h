
/**
 * @file Map.h
 * @brief Declaration of the Map class for the Tower Defense game.
 *
 * This file declares the Map class that manages a grid-based game map.
 * The map consists of cells that can be of various types (SCENERY, PATH, TOWER, CRITTER).
 * It provides functions to configure the map (e.g., setting entry and exit points),
 * validate its configuration, and display the map and a calculated entity path.
 *
 * @author 
 * @date 
 */

#ifndef MAP_H
#define MAP_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

/**
 * @class Map
 * @brief Represents a grid-based map used in the Tower Defense game.
 *
 * The Map class maintains a 2D grid of cells. Each cell has a type (SCENERY, PATH, TOWER, or CRITTER)
 * and flags to indicate if it is the entry or exit point. The class supports setting cell types,
 * defining entry/exit points, validating the connectivity of the map using BFS, and displaying
 * the map and the path from entry to exit.
 */
class Map {
private:
  /// Enumeration of possible cell types.
  enum CellType { SCENERY, PATH, TOWER, CRITTER };

  /**
   * @struct Cell
   * @brief Represents a single cell in the map.
   *
   * Each Cell has a type and two boolean flags indicating if the cell is the entry or exit point.
   */
  struct Cell {
    CellType type;      ///< The type of the cell.
    bool isEntry = false; ///< True if this cell is the entry point.
    bool isExit = false;  ///< True if this cell is the exit point.
  };

  int width, height;  ///< Dimensions of the map.
  std::vector<std::vector<Cell>> grid; ///< 2D grid of cells.
  std::pair<int, int> entryPoint = {-1, -1}; ///< Coordinates of the entry point.
  std::pair<int, int> exitPoint = {-1, -1};  ///< Coordinates of the exit point.

  std::vector<std::pair<int, int>> entityPath; ///< Stores the path from entry to exit.

  /**
   * @brief Performs a breadth-first search (BFS) to find a path from the entry to the exit.
   *
   * If a valid path exists, it stores the path in entityPath.
   *
   * @return true if a valid path is found, false otherwise.
   */
  bool bfsFindPath();

public:
  /**
   * @brief Constructs a new Map object.
   *
   * Initializes the map grid with the specified dimensions and sets all cells to SCENERY.
   *
   * @param w The width (number of columns) of the map.
   * @param h The height (number of rows) of the map.
   */
  Map(int w, int h);

  /**
   * @brief Retrieves the entry point of the map.
   *
   * @return A pair containing the row and column indices of the entry point.
   */
  std::pair<int, int> getEntryPoint() { return entryPoint; }
  
  /**
   * @brief Retrieves the exit point of the map.
   *
   * @return A pair containing the row and column indices of the exit point.
   */
  std::pair<int, int> getExitPoint() { return exitPoint; }

  /**
   * @brief Retrieves the height of the map.
   *
   * @return The number of rows in the map.
   */
  int getHeight() { return height; }
  
  /**
   * @brief Retrieves the width of the map.
   *
   * @return The number of columns in the map.
   */
  int getWidth() { return width; }

  /**
   * @brief Determines if the cell at (x, y) is a critter cell.
   *
   * @param x The row index.
   * @param y The column index.
   * @return true if the cell is of type CRITTER, false otherwise.
   */
  bool isCritterCell(int x, int y);

  /**
   * @brief Checks if the coordinate (x, y) is within the bounds of the map.
   *
   * @param x The row index.
   * @param y The column index.
   * @return true if the coordinate is valid, false otherwise.
   */
  bool isValidCoordinate(int x, int y);

  /**
   * @brief Checks if the cell at (x, y) is a path cell.
   *
   * @param x The row index.
   * @param y The column index.
   * @return true if the cell is a PATH cell, false otherwise.
   */
  bool isPathCell(int x, int y);

  /**
   * @brief Determines if moving from (x0, y0) to (x1, y1) brings you closer to the exit.
   *
   * @param x0 The current x-coordinate.
   * @param x1 The new x-coordinate.
   * @param y0 The current y-coordinate.
   * @param y1 The new y-coordinate.
   * @return true if the new position is closer to the exit, false otherwise.
   */
  bool isCloserToExit(int x0, int x1, int y0, int y1);

  /**
   * @brief Sets the cell at (x, y) to be a path cell.
   *
   * @param x The row index.
   * @param y The column index.
   */
  void setCellToPath(int x, int y);

  /**
   * @brief Sets the cell at (x, y) to be a scenery cell.
   *
   * @param x The row index.
   * @param y The column index.
   */
  void setCellToScenery(int x, int y);

  /**
   * @brief Sets the cell at (x, y) to be occupied by a tower.
   *
   * @param x The row index.
   * @param y The column index.
   */
  void setCellToOccupied(int x, int y);

  /**
   * @brief Sets the entry point of the map.
   *
   * The entry point must be on a valid PATH cell.
   *
   * @param x The row index for the entry.
   * @param y The column index for the entry.
   */
  void setEntryPoint(int x, int y);

  /**
   * @brief Sets the exit point of the map.
   *
   * The exit point must be on a valid PATH cell.
   *
   * @param x The row index for the exit.
   * @param y The column index for the exit.
   */
  void setExitPoint(int x, int y);

  /**
   * @brief Sets the cell at (x, y) to be a critter cell.
   *
   * @param x The row index.
   * @param y The column index.
   */
  void setCellToCritterCell(int x, int y);

  /**
   * @brief Checks if the cell at (x_val, y_val) is a scenery cell.
   *
   * @param x_val The row index.
   * @param y_val The column index.
   * @return true if the cell is a SCENERY cell; false otherwise.
   */
  bool isSceneryCell(int x_val, int y_val);

  /**
   * @brief Validates the map configuration.
   *
   * Ensures that the entry and exit points are set and valid, and that a path exists between them.
   *
   * @return true if the map is valid; false otherwise.
   */
  bool validateMap();

  /**
   * @brief Displays the map on the console.
   *
   * The map is displayed with row labels (letters) and column labels (digits).
   */
  void displayMap();

  /**
   * @brief Displays the calculated entity path from the entry to the exit.
   */
  void displayEntityPath();
};

#endif // MAP_H
