#include "MapObserver.h"
#include "../Maps/Map.h"
#include "Subject.h"
#include <iostream>
#include <utility>
#include <vector>

void MapObserver::Update(Subject *subject) {
  Map *map = dynamic_cast<Map *>(subject);

  std::cout << "------------------MAP OBSERVER----------------------\n\n"
            << std::endl;

  std::cout << "Map dimensions = " << map->gridHeight << "x" << map->gridWidth
            << "\n";

  std::vector<std::pair<int, int>> path = map->getPath();

  std::cout << "Entry = (" << map->entryRow << ", " << map->entryCol << ")"
            << std::endl;

  std::cout << "Exit = (" << map->exitRow << ", " << map->exitCol << ")"
            << std::endl;

  std::cout << "Path = [";
  for (int i = 0; i < path.size(); i++) {
    std::cout << "(" << path[i].first << ", " << path[i].second << ") ";
  }

  std::cout << "]";

  std::cout << "\nTowers positions = [\n";

  for (int i = 0; i < map->gridHeight; i++) {
    for (int j = 0; j < map->gridHeight; j++) {
      if (map->grid[i][j] == REGULARTOWER) {
        std::cout << "(REGULARTOWER, row = " << i << ", col = " << j << ") \n";
      } else if (map->grid[i][j] == SNIPERTOWER) {
        std::cout << "(SNIPERTOWER, row = " << i << ", col = " << j << ") \n";
      } else if (map->grid[i][j] == BOMBTOWER) {
        std::cout << "(BOMBTOWER, row = " << i << ", col = " << j << ") \n";
      } else if (map->grid[i][j] == FREEZINGTOWER) {
        std::cout << "(FREEZINGTOWER, row = " << i << ", col = " << j << ") \n";
      }
    }
  }
  std::cout << "]" << std::endl;

  std::cout << "\nCritters positions = [\n";

  for (int i = 0; i < map->gridHeight; i++) {
    for (int j = 0; j < map->gridWidth; j++) {
      if (map->grid[i][j] == SQUIRRELCRITTER)
        std::cout << "SQUIRREL, (" << i << ", " << j << ")" << std::endl;
      else if (map->grid[i][j] == WOLFCRITTER)
        std::cout << "WOLF, (" << i << ", " << j << ")" << std::endl;
      else if (map->grid[i][j] == BEARCRITTER)
        std::cout << "BEAR, (" << i << ", " << j << ")" << std::endl;
    }
  }

  std::cout << "]" << std::endl;

  std::cout << "------------------END MAP OBSERVER----------------------\n\n"
            << std::endl;
}
