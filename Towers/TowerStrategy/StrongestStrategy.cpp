
#include "StrongestStrategy.h"
#include "../../Critters/Critter.h"
#include "../Tower.h"
#include <cmath>

Critter *
StrongestTargetStrategy::selectTarget(const std::vector<Critter *> &critters,
                                      Tower *tower) {
  int towerX = tower->getX();
  int towerY = tower->getY();
  Critter *selected = nullptr;
  int range = tower->getRange();
  int maxStrength = -1;

  for (Critter *critter : critters) {
    int row = critter->getRow();
    int col = critter->getCol();
    int dx = towerX - row;
    int dy = towerY - col;
    int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
    if (distance <= range) {
      int strength = critter->getStr();
      if (strength > maxStrength) {
        maxStrength = strength;
        selected = critter;
      }
    }
  }
  return selected;
}
