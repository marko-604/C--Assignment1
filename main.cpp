#include "Critter/critter.h"
#include "Map/Map.h"
#include "towers/towers.h"
#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

std::vector<Critter *> gameCritters;
int main() {
  // -------------------------------------------------------------------------------------------
  //                                         MAP
  // -------------------------------------------------------------------------------------------

  int width, height;
  cout << "Enter map width: ";
  cin >> width;
  cout << "Enter map height: ";
  cin >> height;

  Map gameMap(width, height);
  gameMap.displayMap();

  char row;
  int col;
  cout << "Enter a set of coordinates in the format \"A0 A1 B1\" ... enter in "
          "exactly this format  (enter Z "
          "-1 to stop):\n";
  while (true) {
    cin >> row >> col;
    if (row == 'Z' && col == -1)
      break;
    gameMap.setCellToPath(row - 'A', col);
    gameMap.displayMap();
  }
  cout << "Set entry point (Row(A-Z) Column(0-9)): ";
  cin >> row >> col;
  gameMap.setEntryPoint(row - 'A', col);
  gameMap.displayMap();

  cout << "Set exit point (Row(A-Z) Column(0-9)): ";
  cin >> row >> col;
  gameMap.setExitPoint(row - 'A', col);
  gameMap.displayMap();

  if (gameMap.validateMap()) {
    cout << "Map is valid!" << endl;
    gameMap.displayEntityPath();

    // TODO do stuff with the towers to show how they work.
    // Need for there to be a critter in range to show how the attacking works
    // and how the tower will acquire a target.

    std::vector<Tower *> gameTowers;

    int points = 300;
    while (points >= 100) {
      cout << "Current number of points: " << points << std::endl;
      cout << "1. Regular(100)" << "\n2. Sniper(150)"
           << "\n3. Bomb(175)\n4. Freezing(200)" << "\n5. Exit" << std::endl;

      int choice;
      cin >> choice;
      if (choice == 1) {
        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;

        while (1) {
          if (!gameMap.isValidCoordinate(row - 'A', col) ||
              gameMap.isPathCell(row - 'A', col)) {
            cout << "Coordinates are not valid please re-enter them: ";
            cin >> row >> col;

          } else {
            break;
          }
        }

        Tower *t = new Tower(row - 'A', col);

        if (t->getCost() <= points) {
          points -= t->getCost();
          cout << points << " left to spend" << std::endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << std::endl;
          delete t;
        }

        gameMap.displayMap();
      } else if (choice == 2) {
        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;

        while (1) {
          if (!gameMap.isValidCoordinate(row - 'A', col) ||
              gameMap.isPathCell(row - 'A', col)) {
            cout << "Coordinates are not valid please re-enter them: ";
            cin >> row >> col;

          } else {
            break;
          }
        }

        Tower *t = new SniperTower(row - 'A', col);

        if (t->getCost() <= points) {

          points -= t->getCost();
          cout << points << " left to spend" << std::endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << std::endl;
          delete t;
        }
        gameMap.displayMap();

      } else if (choice == 3) {

        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;

        while (1) {
          if (!gameMap.isValidCoordinate(row - 'A', col) ||
              gameMap.isPathCell(row - 'A', col)) {
            cout << "Coordinates are not valid please re-enter them: ";
            cin >> row >> col;

          } else {
            break;
          }
        }

        Tower *t = new BombTower(row - 'A', col);

        if (t->getCost() <= points) {
          points -= t->getCost();
          cout << points << " left to spend" << std::endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << std::endl;
          delete t;
        }
        gameMap.displayMap();
      } else if (choice == 4) {

        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;

        while (1) {
          if (!gameMap.isValidCoordinate(row - 'A', col) ||
              gameMap.isPathCell(row - 'A', col)) {
            cout << "Coordinates are not valid please re-enter them: ";
            cin >> row >> col;

          } else {
            break;
          }
        }

        Tower *t = new FreezingTower(row - 'A', col);

        if (t->getCost() <= points) {
          points -= t->getCost();
          cout << points << " left to spend" << std::endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << std::endl;
          delete t;
        }
        gameMap.displayMap();
      } else {
        break;
      }
    }

    // How do we go from here to complete this code?

    int tick_interval = 1;
    int tick_count = 0;
    int total_tick_count = 60;

    CritterWaveGenerator *generator = new CritterWaveGenerator(
        1, gameMap.getEntryPoint().first, gameMap.getEntryPoint().second);

    int playerHealth = 500;
    while (tick_count <= total_tick_count) {
      tick_count += 1;
      cout << "Tick " << tick_count << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(tick_interval));

      if (tick_count % 10 == 0 || tick_count == 1) {
        gameCritters = generator->generateWave();
        generator->incrementWaveNumber();
      }

      for (Critter *c : gameCritters) {

        c->incrementAccumulator();

        if (c->getAccumulator() == c->getSpeed()) {
          c->setAccumulator(0);

          if (c->getX() == gameMap.getExitPoint().first &&
              c->getY() == gameMap.getExitPoint().second) {

            if (playerHealth >= 0) {
              playerHealth = playerHealth - c->getStrength();
              cout << "Player has " << playerHealth << std::endl;
              if (playerHealth <= 0) {
                cout << "PLAYER is DEAD!! GAME OVER" << std::endl;
                return 0;
              }
            }
          }
          c->move(&gameMap);
        }
      }

      for (Tower *t : gameTowers) {

        t->increment_accumulator();

        if (t->getAccumulator() == t->getFireRate()) {
          t->setAccumulator(0);
          Critter *c = t->inRange(gameCritters);

          if (c == nullptr) {
            continue;
          } else {
            // We attack the target.
            t->attack(c);
            if (c->isDead()) {
              points += c->getValue();
              cout << "Player has gained " << c->getValue()
                   << " points now player has " << points << endl;

              // Get the position of the critter in the vector than erase it
              // from the vector.
              auto it = std::find(gameCritters.begin(), gameCritters.end(), c);
              if (it != gameCritters.end()) {
                // Delete the critter memory:
                delete *it;
                // Remove the pointer from the vector:
                gameCritters.erase(it);
              }
            }
          }
        }
      }

      gameMap.displayMap();
    }
  } else {
    cout << "Map is invalid!" << endl;
  }
  // -------------------------------------------------------------------------------------------

  return 0;
}
