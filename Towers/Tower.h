#ifndef TOWERS_H
#define TOWERS_H

#include "../Observer/Subject.h"
#include "TowerStrategy/SelectionStrategy.h"
#include "TowerStrategy/Strategies.h"
#include "raylib.h"
#include <iostream>
#include <vector>

// Forward declarations:
class Map;
class Critter;

/// @brief Enum representing different types of towers.
enum TowerType { REGULAR, SNIPER, BOMB, FREEZING };

/// @brief Enum representing different tower targeting strategies.
enum TowerStrategy { WEAKEST, CLOSEST, STRONGEST, HEALTHIEST, LEASTHEALTHY };

/// @brief Represents a Tower that can attack Critters using a specified
/// strategy.
class Tower : public Subject {
public:
  /**
   * @brief Construct a new Tower object with full attributes.
   *
   * @param x_val X position on the map grid.
   * @param y_val Y position on the map grid.
   * @param cost_val Initial cost to place the tower.
   * @param damage_val Damage per attack.
   * @param attack_rate_val Attack cooldown rate (ticks between attacks).
   * @param hit_rate_val Probability to hit a target (0.0 to 1.0).
   * @param range_val Maximum attack range.
   * @param type_val TowerType (e.g., REGULAR, SNIPER).
   * @param level_val Starting upgrade level.
   * @param level_up_cost_val Cost to upgrade.
   * @param resale_val Value returned when selling the tower.
   * @param strat Pointer to the targeting strategy.
   */
  Tower(int x_val, int y_val, int cost_val, int damage_val, int attack_rate_val,
        float hit_rate_val, int range_val, TowerType type_val, int level_val,
        int level_up_cost_val, int resale_val, TargetSelectionStrategy *strat);

  /// @brief Default constructor.
  Tower();

  /// @brief Virtual destructor for polymorphic cleanup.
  virtual ~Tower();

  // ------------------- Getters -------------------

  /// @brief Get the unique Tower ID.
  virtual int getTid();

  /// @brief Get the tower's X coordinate on the grid.
  virtual int getX();

  /// @brief Get the tower's Y coordinate on the grid.
  virtual int getY();

  /// @brief Get the tower's initial placement cost.
  virtual int getCost();

  /// @brief Get the damage value per attack.
  virtual int getDamage();

  /// @brief Get the attack rate (ticks between attacks).
  virtual int getAttaRate();

  /// @brief Get the probability to hit (0.0 to 1.0).
  virtual float getHitRate();

  /// @brief Get the range of the tower's attack.
  virtual int getRange();

  /// @brief Get the resale value.
  virtual int getResale();

  /// @brief Get the tower's current upgrade level.
  virtual int getLevel();

  /// @brief Get the tower's type (REGULAR, SNIPER, etc.).
  virtual TowerType getType();

  /// @brief Get the cost to upgrade to the next level.
  virtual int getLevelUpCost();

  // ------------------- Setters -------------------

  /// @brief Set the tower's level.
  virtual void setLevel(int x);

  /// @brief Set the cost to level up.
  virtual void setLevelUpCost(int x);

  /// @brief Set the X coordinate of the tower.
  virtual void setX(int x_val);

  /// @brief Set the Y coordinate of the tower.
  virtual void setY(int y_val);

  /// @brief Set the tower's cost.
  virtual void setCost(int cost_val);

  /// @brief Set the tower's resale value.
  virtual void setResale(int resale_val);

  /// @brief Set the damage value.
  virtual void setDamage(int damage_val);

  /// @brief Set the attack rate.
  virtual void setAttackRate(int attackRate);

  /// @brief Set the hit rate.
  virtual void setHitRate(float hit_rate_val);

  /// @brief Set the range value.
  virtual void setRange(int range);

  // ------------------- Operations -------------------

  /**
   * @brief Increase the tower's level, typically improving stats.
   */
  virtual void levelUp();

  /**
   * @brief Execute an attack on a list of critters if cooldown allows.
   *
   * @param critters List of pointers to available targets.
   * @param tick_count Current game tick (used for cooldown).
   * @param player_points Pointer to player points (for reward updating).
   * @param gameMap Reference to the game map.
   * @return true if an attack was made.
   * @return false otherwise.
   */
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap);

  // ------------------- Strategy Management -------------------

  /**
   * @brief Set a new target selection strategy for the tower.
   *
   * @param strat Pointer to a new strategy object.
   */
  void setStrategy(TargetSelectionStrategy *strat);

  /**
   * @brief Get the current target selection strategy.
   *
   * @return TargetSelectionStrategy* Pointer to the strategy object.
   */
  TargetSelectionStrategy *getStrategy() const;

private:
  TowerType type;                    ///< Type of the tower (e.g., SNIPER).
  int x;                             ///< X position on the map.
  int y;                             ///< Y position on the map.
  int cost;                          ///< Cost to build.
  int resale;                        ///< Value when selling the tower.
  int damage;                        ///< Attack damage.
  int attack_rate;                   ///< Number of ticks between attacks.
  float hit_rate;                    ///< Hit probability.
  int range;                         ///< Attack range.
  int tid;                           ///< Unique tower ID.
  static int nextId;                 ///< Static tracker for assigning IDs.
  int level;                         ///< Upgrade level.
  int levelUpCost;                   ///< Cost to level up.
  TargetSelectionStrategy *strategy; ///< Targeting strategy pointer.
};

#endif // TOWERS_H
