
#include "MapObserver.h"
#include "../Maps/Map.h"
#include "Subject.h"
#include <sstream>

void MapObserver::Update(Subject *subject) {
  // Clear the shared messages vector.
  messages->clear();

  Map *map = dynamic_cast<Map *>(subject);
  if (!map)
    return;

  std::ostringstream oss;

  // Map dimensions
  oss << "Map dimensions = " << map->gridHeight << "x" << map->gridWidth;
  messages->push_back(oss.str());
  oss.str(""); // Clear the string stream

  // Entry point
  oss << "Entry = (" << map->entryRow << ", " << map->entryCol << ")";
  messages->push_back(oss.str());
  oss.str("");

  // Exit point
  oss << "Exit = (" << map->exitRow << ", " << map->exitCol << ")";
  messages->push_back(oss.str());
  oss.str("");

  // Path
  std::vector<std::pair<int, int>> path = map->getPath();
  oss << "Path = [";
  for (size_t i = 0; i < path.size(); i++) {
    oss << "(" << path[i].first << ", " << path[i].second << ") ";
  }
  oss << "]";
  messages->push_back(oss.str());
  oss.str("");

  // Towers positions
  messages->push_back("Towers positions = [");
  for (int i = 0; i < map->gridHeight; i++) {
    for (int j = 0; j < map->gridWidth; j++) {
      if (map->grid[i][j] == REGULARTOWER) {
        oss << "(REGULARTOWER, row = " << i << ", col = " << j << ")";
        messages->push_back(oss.str());
        oss.str("");
      } else if (map->grid[i][j] == SNIPERTOWER) {
        oss << "(SNIPERTOWER, row = " << i << ", col = " << j << ")";
        messages->push_back(oss.str());
        oss.str("");
      } else if (map->grid[i][j] == BOMBTOWER) {
        oss << "(BOMBTOWER, row = " << i << ", col = " << j << ")";
        messages->push_back(oss.str());
        oss.str("");
      } else if (map->grid[i][j] == FREEZINGTOWER) {
        oss << "(FREEZINGTOWER, row = " << i << ", col = " << j << ")";
        messages->push_back(oss.str());
        oss.str("");
      }
    }
  }
  messages->push_back("]");

  // Critters positions
  messages->push_back("Critters positions = [");
  for (int i = 0; i < map->gridHeight; i++) {
    for (int j = 0; j < map->gridWidth; j++) {
      if (map->grid[i][j] == SQUIRRELCRITTER) {
        oss << "SQUIRREL, (" << i << ", " << j << ")";
        messages->push_back(oss.str());
        oss.str("");
      } else if (map->grid[i][j] == WOLFCRITTER) {
        oss << "WOLF, (" << i << ", " << j << ")";
        messages->push_back(oss.str());
        oss.str("");
      } else if (map->grid[i][j] == BEARCRITTER) {
        oss << "BEAR, (" << i << ", " << j << ")";
        messages->push_back(oss.str());
        oss.str("");
      }
    }
  }
  messages->push_back("]");
}
