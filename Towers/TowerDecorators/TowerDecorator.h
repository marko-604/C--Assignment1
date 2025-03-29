#ifndef TOWER_DECORATOR_H
#define TOWER_DECORATOR_H

#include "../Tower.h"
#include <vector>

/**
 * @brief Abstract decorator class for towers.
 *
 * Allows additional functionality to be dynamically added to a Tower
 * by wrapping it. Inherits from Tower and delegates calls to the base tower.
 */
class TowerDecorator : public Tower {
protected:
  Tower *baseTower; ///< The wrapped Tower instance.

public:
  /**
   * @brief Constructs a TowerDecorator that wraps a given tower.
   *
   * @param tower Pointer to the base Tower to be wrapped.
   */
  TowerDecorator(Tower *tower);

  /// @brief Virtual destructor for proper cleanup.
  virtual ~TowerDecorator();

  // ---------- Forwarding virtual getters ----------

  /// @copydoc Tower::getTid
  virtual int getTid() override;

  /// @copydoc Tower::getX
  virtual int getX() override;

  /// @copydoc Tower::getY
  virtual int getY() override;

  /// @copydoc Tower::getCost
  virtual int getCost() override;

  /// @copydoc Tower::getDamage
  virtual int getDamage() override;

  /// @copydoc Tower::getAttaRate
  virtual int getAttaRate() override;

  /// @copydoc Tower::getHitRate
  virtual float getHitRate() override;

  /// @copydoc Tower::getRange
  virtual int getRange() override;

  /// @copydoc Tower::getResale
  virtual int getResale() override;

  /// @copydoc Tower::getLevel
  virtual int getLevel() override;

  /// @copydoc Tower::getType
  virtual TowerType getType() override;

  /// @copydoc Tower::getLevelUpCost
  virtual int getLevelUpCost() override;

  // ---------- Forwarding virtual setters ----------

  /// @copydoc Tower::setLevel
  virtual void setLevel(int lvl) override;

  /// @copydoc Tower::setX
  virtual void setX(int x_val) override;

  /// @copydoc Tower::setY
  virtual void setY(int y_val) override;

  /// @copydoc Tower::setCost
  virtual void setCost(int cost_val) override;

  /// @copydoc Tower::setDamage
  virtual void setDamage(int dmg_val) override;

  /// @copydoc Tower::setAttackRate
  virtual void setAttackRate(int attackRate) override;

  /// @copydoc Tower::setHitRate
  virtual void setHitRate(float hit_rate_val) override;

  /// @copydoc Tower::setRange
  virtual void setRange(int range_val) override;

  /// @copydoc Tower::setResale
  virtual void setResale(int resale_val) override;

  /// @copydoc Tower::setLevelUpCost
  virtual void setLevelUpCost(int x) override;

  // ---------- Forwarding operations ----------

  /// @copydoc Tower::levelUp
  virtual void levelUp() override;

  /**
   * @brief Forwards attack logic to the wrapped Tower.
   *
   * @param critters List of critters to attack.
   * @param tick_count Current tick (used for cooldown logic).
   * @param player_points Pointer to player points to update on kill.
   * @param gameMap Reference to the game map.
   * @return true If an attack was made.
   * @return false If no attack occurred.
   */
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // TOWER_DECORATOR_H
