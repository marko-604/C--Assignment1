/**
 * @file critter.h
 * @brief Declaration of the Critter class and its derived classes, as well as the CritterWaveGenerator.
 *
 * This file defines the base class for critters in the Tower Defense game along with specialized critters
 * (Squirrel, Wolf, Bear). It also declares the CritterWaveGenerator for generating waves of critters.
 */

#ifndef CRITTER_H
#define CRITTER_H

#include "../Map/Map.h"
#include <iostream>
#include <string>
#include <vector>

/**
 * @class Critter
 * @brief Represents a moving enemy in the Tower Defense game.
 *
 * A Critter has attributes such as reward value, speed, health, strength, and position.
 * It also has a unique identifier assigned automatically. Methods allow manipulation of its state,
 * movement on the map, and checking its status.
 */
class Critter {
public:
  /**
   * @brief Constructs a new Critter object with the specified parameters.
   *
   * @param val Reward value for defeating this critter.
   * @param spd_val Speed of the critter.
   * @param hlt_val Initial health of the critter.
   * @param str_val Strength (damage value) of the critter.
   * @param x_val Starting x-coordinate.
   * @param y_val Starting y-coordinate.
   */
  Critter(int val, int spd_val, int hlt_val, int str_val, int x_val, int y_val)
      : value(val), tck_accumulator(0), cid(NextCID++), speed(spd_val),
        health(hlt_val), strength(str_val), x(x_val), y(y_val) {};

  /**
   * @brief Retrieves the speed of the critter.
   * @return The critter's speed.
   */
  int getSpeed() { return speed; }

  /**
   * @brief Retrieves the current health of the critter.
   * @return The critter's health.
   */
  int getHealth() { return health; }

  /**
   * @brief Retrieves the strength of the critter.
   * @return The critter's strength.
   */
  int getStrength() { return strength; }

  /**
   * @brief Retrieves the unique identifier of the critter.
   * @return The critter's unique ID.
   */
  int getCID() { return cid; }

  /**
   * @brief Retrieves the reward value of the critter.
   * @return The reward value.
   */
  int getValue() { return value; }

  /**
   * @brief Retrieves the tick accumulator value.
   * @return The current tick accumulator.
   */
  int getAccumulator() { return tck_accumulator; }

  /**
   * @brief Sets the critter's health.
   * @param hlt_val New health value.
   */
  void setHealth(int hlt_val) { health = hlt_val; }

  /**
   * @brief Sets the critter's speed.
   * @param spd_val New speed value.
   */
  void setSpeed(int spd_val) { speed = spd_val; }

  /**
   * @brief Sets the critter's strength.
   * @param str_val New strength value.
   */
  void setStrength(int str_val) { strength = str_val; }

  /**
   * @brief Sets the tick accumulator value.
   * @param val New accumulator value.
   */
  void setAccumulator(int val) { tck_accumulator = val; }

  /**
   * @brief Increments the tick accumulator by 1.
   */
  void incrementAccumulator() { tck_accumulator += 1; }

  /**
   * @brief Displays information about the critter.
   */
  void displayInfo();

  /**
   * @brief Retrieves the current x-coordinate of the critter.
   * @return The x-coordinate.
   */
  int getX() { return x; }

  /**
   * @brief Retrieves the current y-coordinate of the critter.
   * @return The y-coordinate.
   */
  int getY() { return y; }

  /**
   * @brief Moves the critter on the provided game map.
   *
   * This function determines the next position for the critter based on its speed and the map layout.
   *
   * @param gameMap Pointer to the Map object.
   */
  void move(Map *gameMap);

  /**
   * @brief Checks whether the critter is dead.
   *
   * A critter is considered dead if its health is less than or equal to zero.
   *
   * @return true if the critter is dead; false otherwise.
   */
  bool isDead();

private:
  int value;             ///< Reward value for defeating the critter.
  int tck_accumulator;   ///< Accumulator for timing the critter's movement.
  int cid;               ///< Unique identifier for the critter.
  int speed;             ///< Movement speed.
  int health;            ///< Current health.
  int strength;          ///< Strength (damage potential).
  int x;                 ///< Current x-coordinate.
  int y;                 ///< Current y-coordinate.
  static int NextCID;    ///< Static counter to assign unique IDs.
};

/**
 * @class Squirrel
 * @brief A basic critter type with lower attributes.
 *
 * The Squirrel represents an easy enemy with predefined attributes.
 */
class Squirrel : public Critter {
public:
  /**
   * @brief Constructs a new Squirrel critter.
   * @param x_val Starting x-coordinate.
   * @param y_val Starting y-coordinate.
   */
  Squirrel(int x_val, int y_val) : Critter(25, 1, 5, 2, x_val, y_val) {}
};

/**
 * @class Wolf
 * @brief A critter type with moderate attributes.
 *
 * The Wolf has higher reward, speed, and health compared to the Squirrel.
 */
class Wolf : public Critter {
public:
  /**
   * @brief Constructs a new Wolf critter.
   * @param x_val Starting x-coordinate.
   * @param y_val Starting y-coordinate.
   */
  Wolf(int x_val, int y_val) : Critter(75, 2, 10, 5, x_val, y_val) {}
};

/**
 * @class Bear
 * @brief A strong critter type with high health and reward.
 *
 * The Bear is a challenging enemy with significant health and reward value.
 */
class Bear : public Critter {
public:
  /**
   * @brief Constructs a new Bear critter.
   * @param x_val Starting x-coordinate.
   * @param y_val Starting y-coordinate.
   */
  Bear(int x_val, int y_val) : Critter(100, 4, 20, 5, x_val, y_val) {}
};

/**
 * @class CritterWaveGenerator
 * @brief Generates waves of critters for the game.
 *
 * The CritterWaveGenerator class creates a vector of Critter pointers based on the current wave number.
 * The wave number can be incremented to scale the difficulty of subsequent waves.
 */
class CritterWaveGenerator {
public:
  /**
   * @brief Constructs a new CritterWaveGenerator.
   *
   * @param waveNumber The starting wave number (used to scale difficulty).
   * @param startX The starting x-coordinate for new critters (usually the map entry).
   * @param startY The starting y-coordinate for new critters (usually the map entry).
   */
  CritterWaveGenerator(int waveNumber, int startX, int startY)
      : waveNumber(waveNumber), startX(startX), startY(startY) {}

  /**
   * @brief Generates a new wave of critters.
   *
   * The method creates and returns a vector of Critter pointers. The number and type
   * of critters generated may depend on the current wave number.
   *
   * @return A vector containing pointers to the newly generated critters.
   */
  std::vector<Critter *> generateWave();

  /**
   * @brief Increments the current wave number.
   *
   * This method is used to scale the difficulty for subsequent waves.
   */
  void incrementWaveNumber() { waveNumber += 1; }

private:
  int waveNumber; ///< Current wave number.
  int startX;     ///< Starting x-coordinate for critters.
  int startY;     ///< Starting y-coordinate for critters.
};

#endif
