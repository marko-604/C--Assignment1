#include "critter.h"

int Critter::NextCID = 100;

// This is the function that will be used from one cell to another in the grid.
// We move the function one cell at a time the speed of the critter will just be
void Critter::move(Map *gameMap) {

  int testX = x - 1;
  int testY = y;

  if (gameMap->isCloserToExit(x, testX, y, testY) &&
      gameMap->isPathCell(testX, testY)) {
    // We move the critter to be in this square in this case.
    gameMap->setCellToPath(x, y);
    x = testX;
    y = testY;

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
    std::cout << "Critter " << cid << " has moved to (" << x << "," << y
              << ")\n";
    return;
  }
}

void Critter::displayInfo() {
  std::cout << "Critter " << cid << " has speed: " << speed
            << " and strength: " << strength << " and health: " << health
            << std::endl;
}

bool Critter::isDead() {
  if (health <= 0) {
    std::cout << "Critter " << cid << " has died" << std::endl;
    return true;

  } else {
    std::cout << "Critter " << cid << " now has health " << health << std::endl;
    return false;
  }
}
