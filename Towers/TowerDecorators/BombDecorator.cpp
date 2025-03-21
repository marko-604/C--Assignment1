#include "BombDecorator.h"
#include "../../Critters/Critter.h"
#include "../../Maps/Map.h"
#include "../TowerStrategy/Strategies.h" // The interface for strategies.
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

BombDecorator::BombDecorator(Tower *tower, int splashRadiusVal,
                             float splashDamageFactorVal)
    : TowerDecorator(tower), splashRadius(splashRadiusVal),
      splashDamageFactor(splashDamageFactorVal) {}

bool BombDecorator::attack(std::vector<Critter *> &critters, int tick_count,
                           int *player_points, Map &gameMap) {
  if (tick_count % baseTower->getAttaRate() != 0) {
    return false;
  }
  // Use the tower's strategy to select a target.
  Critter *primaryTarget = nullptr;
  if (baseTower->getStrategy() != nullptr) {
    primaryTarget = baseTower->getStrategy()->selectTarget(critters, baseTower);
  }
  // Fallback: if no strategy is set or no target found, try default selection.
  if (primaryTarget == nullptr) {
    std::cout << "NO TARGETING STRATEGY!!\n";
    for (Critter *p : critters) {

      int row = p->getRow();
      int col = p->getCol();
      int current_row = baseTower->getX();
      int current_col = baseTower->getY();
      int distance = static_cast<int>(
          std::sqrt((current_row - row) * (current_row - row) +
                    (current_col - col) * (current_col - col)));
      if (distance <= baseTower->getRange()) {
        primaryTarget = p;
        break;
      }
    }
  }

  // If a primary target was selected, perform the attack.
  if (primaryTarget != nullptr) {
    std::cout << "Targeting strategy acquired\n";
    // Apply primary damage.
    primaryTarget->setHealth(primaryTarget->getHealth() -
                             baseTower->getDamage());
    std::cout << "BombDecorator: Tower " << baseTower->getTid()
              << " attacked primary target." << std::endl;

    // If primary target is eliminated, update map and award points.
    if (primaryTarget->getHealth() <= 0) {
      std::cout << "BombDecorator: Primary target eliminated!" << std::endl;
      std::cout << "HERE3" << std::endl;
      int row = primaryTarget->getRow();
      std::cout << "HERE3" << std::endl;
      int col = primaryTarget->getCol();
      if (row == gameMap.entryRow && col == gameMap.entryCol) {
        gameMap.grid[row][col] = ENTRY;
      } else if (row == gameMap.exitRow && col == gameMap.exitCol) {
        gameMap.grid[row][col] = EXIT;
      } else {
        if (row != -1 && col != -1) {
          gameMap.grid[row][col] = PATH;
        }
      }
      *player_points += primaryTarget->getValue();
      // Remove the primary target from critters (ensure proper iteration in
      // your full code).
      auto it = std::find(critters.begin(), critters.end(), primaryTarget);
      if (it != critters.end() && (*it) != nullptr || (*it) != NULL) {

        std::cout << "HERE1" << std::endl;
        delete *it;
        std::cout << "HERE2" << std::endl;
        critters.erase(it);
      }
    }

    // Apply splash damage around the primary target.
    int targetRow = primaryTarget->getRow();
    int targetCol = primaryTarget->getCol();
    std::cout << "BombDecorator: Applying splash damage around target."
              << std::endl;
    for (auto it = critters.begin(); it != critters.end();) {
      Critter *critter = *it;
      int row = critter->getRow();
      int col = critter->getCol();
      int dx = targetRow - row;
      int dy = targetCol - col;
      int splashDistance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
      if (splashDistance <= splashRadius) {
        int splashDamage =
            static_cast<int>(baseTower->getDamage() * splashDamageFactor);
        critter->setHealth(critter->getHealth() - splashDamage);
        std::cout << "BombDecorator: Critter at (" << row << "," << col
                  << ") takes " << splashDamage << " splash damage."
                  << std::endl;
        if (critter->getHealth() <= 0) {
          std::cout << "BombDecorator: Critter eliminated by splash damage!"
                    << std::endl;
          if (row == gameMap.entryRow && col == gameMap.entryCol) {
            gameMap.grid[row][col] = ENTRY;
          } else if (row == gameMap.exitRow && col == gameMap.exitCol) {
            gameMap.grid[row][col] = EXIT;
          } else {
            if (row != -1 && col != -1)
              gameMap.grid[row][col] = PATH;
          }
          *player_points += critter->getValue();
          std::cout << "HERE1" << std::endl;
          delete critter;
          std::cout << "HERE2" << std::endl;
          it = critters.erase(it);
          continue;
        }
      }
      ++it;
    }
    return true;
  }
  return false;
}
