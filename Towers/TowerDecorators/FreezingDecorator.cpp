#include "FreezingDecorator.h"
#include "../../Critters/Critter.h"
#include "../../Maps/Map.h"
#include <cmath>
#include <iostream>
FreezingDecorator::FreezingDecorator(Tower *tower, float slowRateVal)
    : TowerDecorator(tower), slowRate(slowRateVal) {}

bool FreezingDecorator::attack(std::vector<Critter *> &critters, int tick_count,
                               int *player_points, Map &gameMap) {
  std::cout << "[FreezingDecorator] Entered attack()...\n";

  // 1) Check if we attack on this tick
  if (tick_count % getAttaRate() != 0) {
    std::cout << "[FreezingDecorator] Not this tower's tick to attack.\n";
    return false;
  }
  std::cout << "[FreezingDecorator] Tick is valid for attacking.\n";

  // 2) Select a target: strategy first
  Critter *target = nullptr;
  if (getStrategy() != nullptr) {
    std::cout << "[FreezingDecorator] Strategy present, selecting target...\n";
    target = getStrategy()->selectTarget(critters, this);
    if (target) {
      std::cout << "[FreezingDecorator] Strategy returned a critter at ("
                << target->getRow() << "," << target->getCol()
                << ") with health " << target->getHealth() << "\n";
    } else {
      std::cout << "[FreezingDecorator] Strategy returned no target.\n";
    }
  } else {
    std::cout << "[FreezingDecorator] No strategy set.\n";
  }

  // 3) Fallback: if no target found from strategy, pick the first in range
  if (!target) {
    std::cout
        << "[FreezingDecorator] Checking fallback for first in range...\n";
    int towerX = getX();
    int towerY = getY();
    for (Critter *c : critters) {
      int dx = towerX - c->getRow();
      int dy = towerY - c->getCol();
      int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
      if (distance <= getRange()) {
        target = c;
        std::cout << "[FreezingDecorator] Fallback found critter at ("
                  << target->getRow() << "," << target->getCol()
                  << ") with health " << target->getHealth() << "\n";
        break;
      }
    }
    if (!target) {
      std::cout << "[FreezingDecorator] No critters in range.\n";
      return false;
    }
  }

  // 4) We have a target. Apply damage and freeze.
  std::cout << "[FreezingDecorator] Attacking critter at (" << target->getRow()
            << "," << target->getCol()
            << ") with health=" << target->getHealth() << "\n";

  target->setHealth(target->getHealth() - getDamage());
  std::cout << "[FreezingDecorator] Dealt " << getDamage()
            << " damage. New health=" << target->getHealth() << "\n";

  // Apply the freezing slow
  float oldSpeed = target->getSpeed();
  target->setSpeed(oldSpeed + slowRate);
  std::cout << "[FreezingDecorator] Applied slow. Speed changed from "
            << oldSpeed << " to " << target->getSpeed() << "\n";

  // 5) Check if the critter died
  if (target->getHealth() <= 0) {
    std::cout << "[FreezingDecorator] Critter died. Removing from vector...\n";
    int r = target->getRow();
    int c = target->getCol();
    std::cout << "HERE !!!\n\n";

    std::cout << "Critter row: " << r << " Critter column: " << c << std::endl;
    if (r == gameMap.entryRow && c == gameMap.entryCol) {
      gameMap.grid[r][c] = ENTRY;

      std::cout << "HERE3 !!!\n\n";
    } else if (r == gameMap.exitRow && c == gameMap.exitCol) {
      gameMap.grid[r][c] = EXIT;
      std::cout << "HERE4 !!!\n\n";
    } else {
      if (r != -1 && c != -1) {
        // do nothing!
        gameMap.grid[r][c] = PATH;
        std::cout << "HERE5 !!!\n\n";
      }
    }

    std::cout << "HERE2 !!!\n\n";
    (*player_points) += target->getValue();

    // Remove from 'critters'
    auto it = std::find(critters.begin(), critters.end(), target);
    if (it != critters.end()) {
      std::cout
          << "[FreezingDecorator] Found the critter in vector. Deleting...\n";
      delete *it;
      std::cout << "[FreezingDecorator] Freed memory. Erasing from vector...\n";
      it = critters.erase(it);
      std::cout << "[FreezingDecorator] Erased from vector.\n";
    } else {
      std::cout << "[FreezingDecorator] WARNING: Critter not found in vector. "
                   "Possibly removed elsewhere!\n";
    }
  }

  std::cout << "[FreezingDecorator] Attack complete.\n";
  return true;
}
