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

// This function returns a wave size based on the wave number
std::vector<Critter *> CritterWaveGenerator::generateWave() {
  std::vector<Critter *> wave;

  for (int i = 0; i < 10 * waveNumber; i++) {
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
