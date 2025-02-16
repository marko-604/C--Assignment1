
/**
 * @file critter.cpp
 * @brief Implementation of Critter class methods and the CritterWaveGenerator.
 *
 * This file defines the movement, display, and status-checking methods for
 * Critter objects, as well as the generation of waves of critters using the
 * CritterWaveGenerator.
 */

#include "critter.h"

/// Initialize the static member for unique critter IDs.
int Critter::NextCID = 100;

/**
 * @brief Moves the critter to an adjacent cell that brings it closer to the
 * exit.
 *
 * The function attempts to move the critter one cell at a time by checking
 * cells in the order left, right, up, and down. For each direction, it uses the
 * map's isCloserToExit() and isPathCell() functions to determine if the new
 * position is valid. If a valid move is found, the critter's current cell is
 * reset to a path cell, the critter's coordinates are updated, and the new cell
 * is marked as a critter cell.
 *
 * @param gameMap Pointer to the Map object representing the game grid.
 */
void Critter::move(Map *gameMap) {

  int testX = x - 1;
  int testY = y;

  if (gameMap->isCloserToExit(x, testX, y, testY) &&
      gameMap->isPathCell(testX, testY)) {
    // Update current cell back to path before moving.
    gameMap->setCellToPath(x, y);
    x = testX;
    y = testY;
    gameMap->setCellToCritterCell(x, y);
    std::cout << "Critter " << cid << " has moved to (" << x << "," << y
              << ")\n";
    return;
  }

  testX = x + 1;
  testY = y;
  if (gameMap->isCloserToExit(x, testX, y, testY) &&
      gameMap->isPathCell(testX, testY)) {
    gameMap->setCellToPath(x, y);
    x = testX;
    y = testY;
    gameMap->setCellToCritterCell(x, y);
    std::cout << "Critter " << cid << " has moved to (" << x << "," << y
              << ")\n";
    return;
  }

  testX = x;
  testY = y - 1;
  if (gameMap->isCloserToExit(x, testX, y, testY) &&
      gameMap->isPathCell(testX, testY)) {
    gameMap->setCellToPath(x, y);
    x = testX;
    y = testY;
    gameMap->setCellToCritterCell(x, y);
    std::cout << "Critter " << cid << " has moved to (" << x << "," << y
              << ")\n";
    return;
  }

  testX = x;
  testY = y + 1;
  if (gameMap->isCloserToExit(x, testX, y, testY) &&
      gameMap->isPathCell(testX, testY)) {
    gameMap->setCellToPath(x, y);
    x = testX;
    y = testY;
    gameMap->setCellToCritterCell(x, y);
    std::cout << "Critter " << cid << " has moved to (" << x << "," << y
              << ")\n";
    return;
  }
}

/**
 * @brief Displays the critter's information.
 *
 * Outputs the critter's unique identifier, speed, strength, and health.
 */
void Critter::displayInfo() {
  std::cout << "Critter " << cid << " has speed: " << speed
            << " and strength: " << strength << " and health: " << health
            << std::endl;
}

/**
 * @brief Checks if the critter is dead.
 *
 * The critter is considered dead if its health is less than or equal to zero.
 * Outputs the critter's status and returns a boolean value accordingly.
 *
 * @return true if the critter's health is 0 or less; false otherwise.
 */
bool Critter::isDead() {
  if (health <= 0) {
    std::cout << "Critter " << cid << " has died" << std::endl;
    return true;
  } else {
    std::cout << "Critter " << cid << " now has health " << health << std::endl;
    return false;
  }
}

/**
 * @brief Generates a wave of critters based on the current wave number.
 *
 * Creates a vector of Critter pointers where the number and type of critters
 * depend on the current wave number. For each iteration, different critter
 * types (Squirrel, Wolf, Bear) are added based on the remainder when dividing
 * the loop counter by 2, 3, or 5.
 *
 * @return A vector containing pointers to the newly generated critters.
 */
std::vector<Critter *> CritterWaveGenerator::generateWave() {
  std::vector<Critter *> wave;

  for (int i = 0; i < 5 * waveNumber; i++) {
    if (i % 2 == 0) {
      Squirrel *t = new Squirrel(startX, startY);
      wave.push_back(t);
    }
    if (i % 3 == 0) {
      Wolf *w = new Wolf(startX, startY);
      wave.push_back(w);
    }
    if (i % 5 == 0) {
      Bear *b = new Bear(startX, startY);
      wave.push_back(b);
    }
  }

  return wave;
}
