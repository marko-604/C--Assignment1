#include "SniperDecorator.h"
#include "../../Critters/Critter.h"
#include "../../Maps/Map.h"
#include <algorithm>
#include <cmath>
#include <iostream>

SniperDecorator::SniperDecorator(Tower *tower, int extraRangeVal,
                                 int extraDamageVal)
    : TowerDecorator(tower), extraRange(extraRangeVal),
      extraDamage(extraDamageVal) {}

bool SniperDecorator::attack(std::vector<Critter *> &critters, int tick_count,
                             int *player_points, Map &gameMap) {
  // Only attack on the appropriate tick.
  if (tick_count % baseTower->getAttaRate() != 0)
    return false;

  int towerX = baseTower->getX();
  int towerY = baseTower->getY();
  int effectiveRange = baseTower->getRange() + extraRange;

  // Choose target: for example, pick the farthest critter within effective
  // range.
  Critter *target = nullptr;
  int maxDistance = -1;
  for (Critter *critter : critters) {
    int row = critter->getRow();
    int col = critter->getCol();
    int dx = towerX - row;
    int dy = towerY - col;
    int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
    if (distance <= effectiveRange && distance > maxDistance) {
      maxDistance = distance;
      target = critter;
    }
  }

  if (target) {
    int totalDamage = baseTower->getDamage() + extraDamage;
    target->setHealth(target->getHealth() - totalDamage);
    std::cout << "SniperDecorator: Tower " << baseTower->getTid()
              << " sniped a critter for " << totalDamage << " damage."
              << std::endl;

    if (target->getHealth() <= 0) {
      std::cout << "SniperDecorator: Critter eliminated!" << std::endl;
      int row = target->getRow();
      int col = target->getCol();
      if (row == gameMap.entryRow && col == gameMap.entryCol) {
        gameMap.grid[row][col] = ENTRY;
      } else if (row == gameMap.exitRow && col == gameMap.exitCol) {
        gameMap.grid[row][col] = EXIT;
      } else {
        gameMap.grid[row][col] = PATH;
      }
      (*player_points) += target->getValue();
      auto it = std::find(critters.begin(), critters.end(), target);
      if (it != critters.end()) {
        delete *it;
        critters.erase(it);
      }
    }
    return true;
  }
  return false;
}
