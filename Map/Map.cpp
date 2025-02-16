
/**
 * @file Map.cpp
 * @brief Implementation of the Map class for the Tower Defense game.
 *
 * This file implements methods for creating and managing a grid-based map.
 * The map supports different cell types (scenery, path, tower, critter) and
 * provides functionality to set entry and exit points, validate the map,
 * and display the map and the calculated entity path.
 *
 * @author 
 * @date 
 */

#include "Map.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

/**
 * @brief Checks if the cell at (x, y) is a critter cell.
 * 
 * @param x The row index.
 * @param y The column index.
 * @return true if the cell is of type CRITTER and within bounds; false otherwise.
 */
bool Map::isCritterCell(int x, int y) {
  if (isValidCoordinate(x, y)) {
    if (grid[x][y].type == CRITTER) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Constructs a new Map object.
 * 
 * Initializes a grid with the given width and height, setting all cells to SCENERY.
 * 
 * @param w The width (number of columns) of the map.
 * @param h The height (number of rows) of the map.
 */
Map::Map(int w, int h) : width(w), height(h) {
  grid.resize(height, vector<Cell>(width, {SCENERY}));
}

/**
 * @brief Checks if the coordinate (x, y) is within the map bounds.
 * 
 * @param x The row index.
 * @param y The column index.
 * @return true if (x, y) is within bounds; false otherwise.
 */
bool Map::isValidCoordinate(int x, int y) {
  return x >= 0 && x < height && y >= 0 && y < width;
}

/**
 * @brief Sets the cell at (x, y) to be a path cell.
 * 
 * If the coordinate is valid, updates the cell's type to PATH. Otherwise, prints an error.
 * 
 * @param x The row index.
 * @param y The column index.
 */
void Map::setCellToPath(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = PATH;
  } else {
    cerr << "Invalid coordinate." << endl;
  }
}

/**
 * @brief Sets the cell at (x, y) to be occupied by a tower.
 * 
 * If the coordinate is valid, updates the cell's type to TOWER. Otherwise, prints an error.
 * 
 * @param x The row index.
 * @param y The column index.
 */
void Map::setCellToOccupied(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = TOWER;
  } else {
    cerr << "Invalid coordinate" << endl;
  }
}

/**
 * @brief Sets the cell at (x, y) to be a scenery cell.
 * 
 * If the coordinate is valid, updates the cell's type to SCENERY. Otherwise, prints an error.
 * 
 * @param x The row index.
 * @param y The column index.
 */
void Map::setCellToScenery(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = SCENERY;
  } else {
    cerr << "Invalid coordinate." << endl;
  }
}

/**
 * @brief Sets the entry point for the map.
 * 
 * The entry point must be a valid coordinate and its cell type must be PATH.
 * Marks the cell as an entry cell.
 * 
 * @param x The row index for the entry.
 * @param y The column index for the entry.
 */
void Map::setEntryPoint(int x, int y) {
  if (isValidCoordinate(x, y) && grid[x][y].type == PATH) {
    entryPoint = {x, y};
    grid[x][y].isEntry = true;
  } else {
    cerr << "Invalid entry." << endl;
  }
}

/**
 * @brief Checks if the cell at (x_val, y_val) is a scenery cell.
 * 
 * First checks if the coordinates are within bounds, then verifies if the cell's type is SCENERY.
 * 
 * @param x_val The row index.
 * @param y_val The column index.
 * @return true if the cell is SCENERY; false otherwise.
 */
bool Map::isSceneryCell(int x_val, int y_val) {
  if (!isValidCoordinate(x_val, y_val)) {
    std::cerr << "Coordinates (" << x_val << ", " << y_val
              << ") are out of bounds." << std::endl;
    return false;
  }
  return grid[x_val][y_val].type == SCENERY;
}

/**
 * @brief Sets the exit point for the map.
 * 
 * The exit point must be a valid coordinate and its cell type must be PATH.
 * Marks the cell as an exit cell.
 * 
 * @param x The row index for the exit.
 * @param y The column index for the exit.
 */
void Map::setExitPoint(int x, int y) {
  if (isValidCoordinate(x, y) && grid[x][y].type == PATH) {
    exitPoint = {x, y};
    grid[x][y].isExit = true;
  } else {
    cerr << "Invalid exit." << endl;
  }
}

/**
 * @brief Performs a breadth-first search (BFS) to find a valid path from entry to exit.
 * 
 * The function uses BFS to determine if there is a connected series of PATH cells
 * from the entry point to the exit point. If found, it stores the path in \c entityPath.
 * 
 * @return true if a valid path is found; false otherwise.
 */
bool Map::bfsFindPath() {
  vector<vector<bool>> visited(height, vector<bool>(width, false));
  vector<vector<pair<int, int>>> parent(
      height, vector<pair<int, int>>(width, {-1, -1}));

  queue<pair<int, int>> q;
  q.push(entryPoint);
  visited[entryPoint.first][entryPoint.second] = true;

  const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  bool pathFound = false;

  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();

    if (x == exitPoint.first && y == exitPoint.second) {
      pathFound = true;
      break;
    }

    for (const auto &[dx, dy] : directions) {
      int nx = x + dx;
      int ny = y + dy;

      if (isValidCoordinate(nx, ny) && !visited[nx][ny] &&
          grid[nx][ny].type == PATH) {
        visited[nx][ny] = true;
        parent[nx][ny] = {x, y};
        q.push({nx, ny});
      }
    }
  }

  if (!pathFound) {
    return false;
  }

  // Build the entity path from exit to entry, then reverse it.
  entityPath.clear();
  pair<int, int> current = exitPoint;
  while (current != entryPoint) {
    entityPath.push_back(current);
    current = parent[current.first][current.second];
  }
  entityPath.push_back(entryPoint);
  reverse(entityPath.begin(), entityPath.end());

  return true;
}

/**
 * @brief Validates the map configuration.
 * 
 * Checks that the entry and exit points have been set, verifies that their respective
 * flags are set, and confirms that there is a valid path connecting the entry and exit.
 * 
 * @return true if the map is valid; false otherwise.
 */
bool Map::validateMap() {
  if (entryPoint == make_pair(-1, -1) || exitPoint == make_pair(-1, -1)) {
    cerr << "Entry or exit not set." << endl;
    return false;
  }

  if (!grid[entryPoint.first][entryPoint.second].isEntry ||
      !grid[exitPoint.first][exitPoint.second].isExit) {
    cerr << "Entry or exit not valid." << endl;
    return false;
  }

  if (!bfsFindPath()) {
    cerr << "Path is not valid." << endl;
    return false;
  }

  return true;
}

/**
 * @brief Checks if the cell at (x, y) is either a PATH cell or a CRITTER cell.
 * 
 * This is used to determine if a cell is valid for critter movement.
 * 
 * @param x The row index.
 * @param y The column index.
 * @return true if the cell is PATH or CRITTER; false otherwise.
 */
bool Map::isPathCell(int x, int y) {
  if (isValidCoordinate(x, y)) {
    if (grid[x][y].type == PATH || grid[x][y].type == CRITTER)
      return true;
  }
  return false;
}

/**
 * @brief Sets the cell at (x, y) to be a critter cell.
 * 
 * If the coordinate is valid, updates the cell's type to CRITTER.
 * 
 * @param x The row index.
 * @param y The column index.
 */
void Map::setCellToCritterCell(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = CRITTER;
  }
  return;
}

/**
 * @brief Determines if moving from (x0, y0) to (x1, y1) brings one closer to the exit.
 * 
 * Uses the Manhattan distance to compare the two positions relative to the exit point.
 * 
 * @param x0 The current x-coordinate.
 * @param x1 The new x-coordinate.
 * @param y0 The current y-coordinate.
 * @param y1 The new y-coordinate.
 * @return true if the new coordinate is closer to the exit; false otherwise.
 */
bool Map::isCloserToExit(int x0, int x1, int y0, int y1) {
  int exitX = exitPoint.first;
  int exitY = exitPoint.second;
  int d0 = std::abs(exitX - x0) + std::abs(exitY - y0); // Manhattan distance
  int d1 = std::abs(exitX - x1) + std::abs(exitY - y1);
  return d1 < d0;
}

/**
 * @brief Displays the current state of the map.
 * 
 * The map is printed with row labels (letters) and column labels (digits). The cell types are:
 * - E: Entry
 * - X: Exit
 * - P: Path
 * - T: Tower
 * - C: Critter
 * - S: Scenery
 */
void Map::displayMap() {
  cout << "\n   ";
  for (int j = 0; j < width; ++j) {
    cout << " " << j % 10 << " ";
  }
  cout << "\n  " + string(width * 3, '-') + "\n";

  for (int i = 0; i < height; ++i) {
    cout << char('A' + i) << " |";
    for (int j = 0; j < width; ++j) {
      if (grid[i][j].isEntry) {
        cout << " E ";
      } else if (grid[i][j].isExit) {
        cout << " X ";
      } else if (grid[i][j].type == PATH) {
        cout << " P ";
      } else if (grid[i][j].type == TOWER) {
        cout << " T ";
      } else if (grid[i][j].type == CRITTER) {
        cout << " C ";
      } else {
        cout << " S ";
      }
    }
    cout << "|\n";
  }
  cout << "  " + string(width * 3, '-') + "\n";
}

/**
 * @brief Displays the calculated entity path from the entry to the exit.
 * 
 * If a valid path has been stored, the function prints the path in order from the entry
 * (using letter-number notation) to the exit. Otherwise, it notifies that no valid path was stored.
 */
void Map::displayEntityPath() {
  if (entityPath.empty()) {
    cout << "No valid path was stored." << endl;
    return;
  }

  cout << "Entity Path [start to finish]:" << endl;
  for (const auto &[x, y] : entityPath) {
    cout << char('A' + x) << y << " -> ";
  }
  cout << "Exit" << endl;
}
