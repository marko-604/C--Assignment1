
#include "LowestHealthStrategy.h"
#include "../../Critters/Critter.h"
#include "../../Towers/Tower.h"
#include <cmath>
#include <limits>
Critter *
LowestHealthTargetStrategy::selectTarget(const std::vector<Critter *> &critters,
                                         Tower *tower) {
  int towerX = tower->getX();
  int towerY = tower->getY();
  Critter *selected = nullptr;
  int range = tower->getRange();
  float minHealth = std::numeric_limits<float>::max();

  for (Critter *critter : critters) {
    int row = critter->getRow();
    int col = critter->getCol();
    int dx = towerX - row;
    int dy = towerY - col;
    int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
    if (distance <= range) {
      float health = critter->getHealth();
      if (health < minHealth) {
        minHealth = health;
        selected = critter;
      }
    }
  }
  return selected;
}
