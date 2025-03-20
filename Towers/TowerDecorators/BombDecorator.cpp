#include "BombDecorator.h"
#include "../../Critters/Critter.h"
#include "../../Maps/Map.h"
#include <algorithm>
#include <cmath>
#include <iostream>

BombDecorator::BombDecorator(Tower *tower, int splashRadiusVal,
                             float splashDamageFactorVal)
    : TowerDecorator(tower), splashRadius(splashRadiusVal),
      splashDamageFactor(splashDamageFactorVal) {}

bool BombDecorator::attack(std::vector<Critter *> &critters, int tick_count,
                           int *player_points, Map &gameMap) {
  // Call the underlying tower's attack.
  bool attacked =
      baseTower->attack(critters, tick_count, player_points, gameMap);
  if (attacked) {
    int towerX = baseTower->getX();
    int towerY = baseTower->getY();
    std::cout << "BombDecorator: Bomb explosion triggers splash damage."
              << std::endl;
    // Loop over all critters to apply splash damage.
    for (auto it = critters.begin(); it != critters.end();) {
      Critter *critter = *it;
      int row = critter->getRow();
      int col = critter->getCol();
      int dx = towerX - row;
      int dy = towerY - col;
      int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
      if (distance <= splashRadius) {
        // Calculate splash damage based on base tower's damage.
        int splashDamage =
            static_cast<int>(baseTower->getDamage() * splashDamageFactor);
        critter->setHealth(critter->getHealth() - splashDamage);
        std::cout << "BombDecorator: Critter at (" << row << "," << col
                  << ") takes " << splashDamage << " splash damage."
                  << std::endl;
        // If the critter dies, update the map and award points.
        if (critter->getHealth() <= 0) {
          std::cout << "BombDecorator: Critter eliminated by splash damage!"
                    << std::endl;
          if (row == gameMap.entryRow && col == gameMap.entryCol) {
            gameMap.grid[row][col] = ENTRY;
          } else if (row == gameMap.exitRow && col == gameMap.exitCol) {
            gameMap.grid[row][col] = EXIT;
          } else {
            gameMap.grid[row][col] = PATH;
          }
          *player_points += critter->getValue();
          delete critter;
          it = critters.erase(it);
          continue;
        }
      }
      ++it;
    }
  }
  return attacked;
}
