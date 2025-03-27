
#include "LevelingDecorator.h"
#include "../../Critters/Critter.h"
#include "../../Maps/Map.h"
#include <iostream>

LevelUpDecorator::LevelUpDecorator(Tower *baseTower, int levelsToGain)
    : TowerDecorator(baseTower), levelsToGain(levelsToGain),
      hasLeveledUp(false) {
  // Constructor does not level up immediately — we'll do that during the first
  // attack
}

bool LevelUpDecorator::attack(std::vector<Critter *> &critters, int tick_count,
                              int *player_points, Map &gameMap) {
  // If we haven't leveled up yet, do it now (on the first attack call).
  if (!hasLeveledUp) {
    std::cout << "LevelUpDecorator: leveling up base tower " << levelsToGain
              << " times.\n";

    // Call baseTower->levelUp() the specified number of times
    for (int i = 0; i < levelsToGain; i++) {
      baseTower->levelUp();
    }
    hasLeveledUp = true;
  }

  // After leveling up once, just pass through to the base tower's attack
  return baseTower->attack(critters, tick_count, player_points, gameMap);
}
