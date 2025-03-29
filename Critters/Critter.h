#ifndef CRITTER_H
#define CRITTER_H

#include "../Observer/Subject.h"
#include <algorithm>
#include <utility>
#include <vector>

class Map;

/// @brief Enum representing different types of Critters.
enum CritterType { SQUIRREL, WOLF, BEAR };

/// @brief Base class for all Critters in the game.
class Critter : public Subject {
public:
  /**
   * @brief Construct a new Critter object.
   *
   * @param hlt_val Health value.
   * @param spd_val Speed value (ticks per move).
   * @param str_val Strength value.
   * @param row_val Initial row.
   * @param col_val Initial column.
   * @param type_val Type of the critter.
   * @param path_val Path the critter will follow.
   * @param val Value (e.g., points or reward).
   */
  Critter(int hlt_val, int spd_val, int str_val, int row_val, int col_val,
          CritterType type_val, std::vector<std::pair<int, int>> path_val,
          int val);

  /// @brief Virtual destructor.
  virtual ~Critter() {}

  /// @brief Get the type of the critter.
  /// @return CritterType
  CritterType getType();

  /// @brief Get the current row position.
  /// @return int
  int getRow();

  /// @brief Get the current column position.
  /// @return int
  int getCol();

  /// @brief Get the critter's movement speed.
  /// @return int
  int getSpeed();

  /// @brief Get the critter's strength.
  /// @return int
  int getStr();

  /// @brief Get the critter's current health.
  /// @return int
  int getHealth();

  /// @brief Get the critter's unique ID.
  /// @return int
  int getCID();

  /// @brief Get the value of defeating the critter.
  /// @return int
  int getValue();

  /// @brief Get the current index in the critter's path.
  /// @return int
  int getPathIndex();

  /// @brief Set the row position.
  /// @param x New row value.
  void setRow(int x);

  /// @brief Set the column position.
  /// @param y New column value.
  void setCol(int y);

  /// @brief Set the speed.
  /// @param x New speed value.
  void setSpeed(int x);

  /// @brief Set the strength.
  /// @param s New strength value.
  void setStr(int s);

  /// @brief Set the health value.
  /// @param h New health value.
  void setHealth(int h);

  /// @brief Set the path the critter follows.
  /// @param path_val New path as vector of (row, col) pairs.
  void setPath(std::vector<std::pair<int, int>> path_val);

  /// @brief Get the current path.
  /// @return std::vector<std::pair<int, int>>
  std::vector<std::pair<int, int>> getPath();

  /**
   * @brief Updates the critter's position on the map based on the tick count.
   *
   * @param map Reference to the game map.
   * @param tick_count Current game tick.
   */
  void Update(Map &map, int tick_count);

  /**
   * @brief Move the critter to a specified row and column.
   *
   * @param map Reference to the game map.
   * @param row Target row.
   * @param col Target column.
   */
  void move(Map &map, int row, int col);

private:
  int health;                            ///< Health value.
  int speed;                             ///< Speed (ticks between moves).
  int strength;                          ///< Strength value.
  int row;                               ///< Current row position.
  int col;                               ///< Current column position.
  int cid;                               ///< Unique critter ID.
  static int nextCID;                    ///< Static ID tracker for critters.
  std::vector<std::pair<int, int>> path; ///< Path to follow.
  CritterType type;                      ///< Critter type.
  int pathIndex;                         ///< Index of current position in path.
  int value;                             ///< Value when defeated.
};

/**
 * @brief A Squirrel-type Critter.
 */
class Squirrel : public Critter {
public:
  /**
   * @brief Construct a new Squirrel object.
   *
   * @param path Path the squirrel will follow.
   */
  Squirrel(std::vector<std::pair<int, int>> path)
      : Critter(5, 2, 2, -1, -1, SQUIRREL, path, 50) {}
};

/**
 * @brief A Wolf-type Critter.
 */
class Wolf : public Critter {
public:
  /**
   * @brief Construct a new Wolf object.
   *
   * @param path Path the wolf will follow.
   */
  Wolf(std::vector<std::pair<int, int>> path)
      : Critter(8, 3, 4, -1, -1, WOLF, path, 75) {}
};

/**
 * @brief A Bear-type Critter.
 */
class Bear : public Critter {
public:
  /**
   * @brief Construct a new Bear object.
   *
   * @param path Path the bear will follow.
   */
  Bear(std::vector<std::pair<int, int>> path)
      : Critter(12, 5, 5, -1, -1, BEAR, path, 100) {}
};

/**
 * @brief Class responsible for generating Critters.
 */
class CritterGenerator {
private:
  int level; ///< Current level of the generator.

public:
  /// @brief Construct a new CritterGenerator object.
  CritterGenerator();

  std::vector<Critter *> critters; ///< Queue of critters to be deployed.

  /// @brief Check if the critter queue is empty.
  /// @return true if empty, false otherwise.
  bool isEmpty();

  /// @brief Get the next critter in the queue.
  /// @return Pointer to a Critter.
  Critter *getCritter();

  /**
   * @brief Increase the level and generate new critters.
   *
   * @param path Path for the new critters.
   */
  void levelUp(std::vector<std::pair<int, int>> path);

  /**
   * @brief Generate a batch of critters based on the current level.
   *
   * @param path Path to assign to the generated critters.
   */
  void generateCritters(std::vector<std::pair<int, int>> path);
};

#endif // !CRITTER_H
