#include "Map.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

bool Map::isCritterCell(int x, int y) {
  if (isValidCoordinate(x, y)) {
    if (grid[x][y].type == CRITTER) {
      return true;
    }
  }

  return false;
}

Map::Map(int w, int h) : width(w), height(h) {
  grid.resize(height, vector<Cell>(width, {SCENERY}));
}

bool Map::isValidCoordinate(int x, int y) {
  return x >= 0 && x < height && y >= 0 && y < width;
}

void Map::setCellToPath(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = PATH;
  } else {
    cerr << "Invalid coordinate." << endl;
  }
}

void Map::setCellToOccupied(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = TOWER;
  } else {
    cerr << "Invalid coordinate" << endl;
  }
}
void Map::setCellToScenery(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = SCENERY;
  } else {
    cerr << "Invalid coordinate." << endl;
  }
}

void Map::setEntryPoint(int x, int y) {
  if (isValidCoordinate(x, y) && grid[x][y].type == PATH) {
    entryPoint = {x, y};
    grid[x][y].isEntry = true;
  } else {
    cerr << "Invalid entry." << endl;
  }
}

bool Map::isSceneryCell(int x_val, int y_val) {
  // First, check if the coordinates are within bounds.
  if (!isValidCoordinate(x_val, y_val)) {
    std::cerr << "Coordinates (" << x_val << ", " << y_val
              << ") are out of bounds." << std::endl;
    return false;
  }
  // Return true if the cell's type is SCENERY.
  return grid[x_val][y_val].type == SCENERY;
}
void Map::setExitPoint(int x, int y) {
  if (isValidCoordinate(x, y) && grid[x][y].type == PATH) {
    exitPoint = {x, y};
    grid[x][y].isExit = true;
  } else {
    cerr << "Invalid exit." << endl;
  }
}

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

  // exit to entry, then reverse
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

bool Map::isPathCell(int x, int y) {
  if (isValidCoordinate(x, y)) {
    if (grid[x][y].type == PATH || grid[x][y].type == CRITTER)
      return true;
  }

  return false;
}

void Map::setCellToCritterCell(int x, int y) {
  if (isValidCoordinate(x, y)) {
    grid[x][y].type = CRITTER;
  }
  return;
}

bool Map::isCloserToExit(int x0, int x1, int y0, int y1) {
  int exitX = exitPoint.first;
  int exitY = exitPoint.second;
  int d0 = std::abs(exitX - x0) + std::abs(exitY - y0); // Manhattan distance
  int d1 = std::abs(exitX - x1) + std::abs(exitY - y1);
  return d1 < d0;
}

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
