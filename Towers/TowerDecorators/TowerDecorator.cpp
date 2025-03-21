#include "TowerDecorator.h"

// Constructor and destructor.
TowerDecorator::TowerDecorator(Tower *tower) : baseTower(tower) {}

TowerDecorator::~TowerDecorator() { delete baseTower; }

// Forwarding getters.
int TowerDecorator::getTid() { return baseTower->getTid(); }
int TowerDecorator::getX() { return baseTower->getX(); }
int TowerDecorator::getY() { return baseTower->getY(); }
int TowerDecorator::getCost() { return baseTower->getCost(); }
int TowerDecorator::getDamage() { return baseTower->getDamage(); }
int TowerDecorator::getAttaRate() { return baseTower->getAttaRate(); }
float TowerDecorator::getHitRate() { return baseTower->getHitRate(); }
int TowerDecorator::getRange() { return baseTower->getRange(); }
int TowerDecorator::getResale() { return baseTower->getResale(); }
int TowerDecorator::getLevel() { return baseTower->getLevel(); }
TowerType TowerDecorator::getType() { return baseTower->getType(); }
int TowerDecorator::getLevelUpCost() { return baseTower->getLevelUpCost(); }

// Forwarding setters.
void TowerDecorator::setLevel(int lvl) { baseTower->setLevel(lvl); }
void TowerDecorator::setX(int x_val) { baseTower->setX(x_val); }
void TowerDecorator::setY(int y_val) { baseTower->setY(y_val); }
void TowerDecorator::setCost(int cost_val) { baseTower->setCost(cost_val); }
void TowerDecorator::setDamage(int dmg_val) { baseTower->setDamage(dmg_val); }
void TowerDecorator::setAttackRate(int attackRate) {
  baseTower->setAttackRate(attackRate);
}
void TowerDecorator::setLevelUpCost(int x) { baseTower->setLevelUpCost(x); }

void TowerDecorator::setHitRate(float hit_rate_val) {
  baseTower->setHitRate(hit_rate_val);
}
void TowerDecorator::setRange(int range_val) { baseTower->setRange(range_val); }
void TowerDecorator::setResale(int resale_val) {
  baseTower->setResale(resale_val);
}

// Forwarding operations.
void TowerDecorator::levelUp() { baseTower->levelUp(); }
bool TowerDecorator::attack(std::vector<Critter *> &critters, int tick_count,
                            int *player_points, Map &gameMap) {
  return baseTower->attack(critters, tick_count, player_points, gameMap);
}
