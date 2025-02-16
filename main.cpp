

/**
 * @file main.cpp
 * @brief Main driver for the Tower Defense game.
 *
 * This file handles the initialization of the game map, tower placement, and
 * the main game loop. It reads user input for map configuration and tower
 * placement, spawns critters via a wave generator, and processes critter
 * movement and tower attacks during game ticks.
 *
 * Demonstrates the functionality of the game, the way that the game works is
 * that there is a global tick that occurs every tick_interval seconds, critters
 * will move based on the ticks and towers will fire on creatures based on
 * critters being in range and the tick counter.
 *
 * Global variable:
 * - gameCritters: A global vector containing pointers to the critters currently
 * in the game.
 *
 * @author
 * @date
 */

#include "Critter/critter.h"
#include "Map/Map.h"
#include "towers/towers.h"
#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

/// Global vector containing pointers to critters currently active in the game.
std::vector<Critter *> gameCritters;

/**
 * @brief Main function that runs the Tower Defense game.
 *
 * The function performs the following steps:
 *  - Prompts the user for the map dimensions.
 *  - Configures the map by reading a series of coordinates to create a path,
 * then setting the entry and exit points.
 *  - Validates the configured map.
 *  - Allows the user to place towers until points run out.
 *  - Enters the main game loop where:
 *    - Critter waves are generated periodically.
 *    - Each tick, critters move and are checked for reaching the exit
 * (affecting player health).
 *    - Towers check for critters in range and attack them.
 *    - The map is updated and displayed.
 *
 * The game loop ends when the tick count exceeds a predefined limit or the
 * player's health reaches zero.
 *
 * @return int 0 if the game ends normally, or 0 if the player dies.
 */
int main() {
  // -------------------------------------------------------------------------------------------
  //                                         MAP SETUP
  // -------------------------------------------------------------------------------------------
  int width, height;
  cout << "Enter map width: ";
  cin >> width;
  cout << "Enter map height: ";
  cin >> height;

  // Create the game map with the specified dimensions.
  Map gameMap(width, height);
  gameMap.displayMap();

  char row;
  int col;
  cout << "Enter a set of coordinates in the format \"A0 A1 B1\" ... enter in "
          "exactly this format (enter Z -1 to stop):\n";
  while (true) {
    cin >> row >> col;
    if (row == 'Z' && col == -1)
      break;
    // Mark the specified cell as a path cell.
    gameMap.setCellToPath(row - 'A', col);
    gameMap.displayMap();
  }

  // Set the entry point for the critters.
  cout << "Set entry point (Row(A-Z) Column(0-9)): ";
  cin >> row >> col;
  gameMap.setEntryPoint(row - 'A', col);
  gameMap.displayMap();

  // Set the exit point for the critters.
  cout << "Set exit point (Row(A-Z) Column(0-9)): ";
  cin >> row >> col;
  gameMap.setExitPoint(row - 'A', col);
  gameMap.displayMap();

  // Validate the map configuration.
  if (gameMap.validateMap()) {
    cout << "Map is valid!" << endl;
    gameMap.displayEntityPath();

    // -------------------------------------------------------------------------------------------
    //                                        TOWER SETUP
    // -------------------------------------------------------------------------------------------
    std::vector<Tower *> gameTowers;
    int points = 300;
    while (points >= 100) {
      cout << "Current number of points: " << points << endl;
      cout << "1. Regular(100)" << "\n2. Sniper(150)"
           << "\n3. Bomb(175)\n4. Freezing(200)" << "\n5. Exit" << endl;

      int choice;
      cin >> choice;
      if (choice == 1) {
        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;
        while (true) {
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
          cout << points << " left to spend" << endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << endl;
          delete t;
        }
        gameMap.displayMap();
      } else if (choice == 2) {
        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;
        while (true) {
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
          cout << points << " left to spend" << endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << endl;
          delete t;
        }
        gameMap.displayMap();
      } else if (choice == 3) {
        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;
        while (true) {
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
          cout << points << " left to spend" << endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << endl;
          delete t;
        }
        gameMap.displayMap();
      } else if (choice == 4) {
        cout << "Select coordinates to place tower: ";
        char row;
        int col;
        cin >> row >> col;
        while (true) {
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
          cout << points << " left to spend" << endl;
          gameMap.setCellToOccupied(row - 'A', col);
          gameTowers.push_back(t);
        } else {
          cout << "Not enough points!" << endl;
          delete t;
        }
        gameMap.displayMap();
      } else {
        break;
      }
    }

    // -------------------------------------------------------------------------------------------
    //                                         GAME LOOP
    // -------------------------------------------------------------------------------------------
    int tick_interval = 1;
    int tick_count = 0;
    int total_tick_count = 60;

    // Create a critter wave generator starting at the map's entry point.
    CritterWaveGenerator *generator = new CritterWaveGenerator(
        1, gameMap.getEntryPoint().first, gameMap.getEntryPoint().second);

    int playerHealth = 500;
    while (tick_count <= total_tick_count) {
      tick_count += 1;
      cout << "Tick " << tick_count << endl;
      std::this_thread::sleep_for(std::chrono::seconds(tick_interval));

      // Generate a new wave of critters on the first tick or every 10 ticks.
      if (tick_count % 10 == 0 || tick_count == 1) {
        gameCritters = generator->generateWave();
        generator->incrementWaveNumber();
      }

      if (tick_count % 20 == 0) {
        while (points >= 100) {
          cout << "Current number of points: " << points << endl;
          cout << "1. Regular(100)" << "\n2. Sniper(150)"
               << "\n3. Bomb(175)\n4. Freezing(200)" << "\n5. Sell Tower"
               << "\n6. Level Up Tower" << "\n7. Exit" << endl;

          int choice;
          cin >> choice;
          if (choice == 1) {
            cout << "Select coordinates to place tower: ";
            char row;
            int col;
            cin >> row >> col;
            while (true) {
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
              cout << points << " left to spend" << endl;
              gameMap.setCellToOccupied(row - 'A', col);
              gameTowers.push_back(t);
            } else {
              cout << "Not enough points!" << endl;
              delete t;
            }
            gameMap.displayMap();
          } else if (choice == 2) {
            cout << "Select coordinates to place tower: ";
            char row;
            int col;
            cin >> row >> col;
            while (true) {
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
              cout << points << " left to spend" << endl;
              gameMap.setCellToOccupied(row - 'A', col);
              gameTowers.push_back(t);
            } else {
              cout << "Not enough points!" << endl;
              delete t;
            }
            gameMap.displayMap();
          } else if (choice == 3) {
            cout << "Select coordinates to place tower: ";
            char row;
            int col;
            cin >> row >> col;
            while (true) {
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
              cout << points << " left to spend" << endl;
              gameMap.setCellToOccupied(row - 'A', col);
              gameTowers.push_back(t);
            } else {
              cout << "Not enough points!" << endl;
              delete t;
            }
            gameMap.displayMap();
          } else if (choice == 4) {
            cout << "Select coordinates to place tower: ";
            char row;
            int col;
            cin >> row >> col;
            while (true) {
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
              cout << points << " left to spend" << endl;
              gameMap.setCellToOccupied(row - 'A', col);
              gameTowers.push_back(t);
            } else {
              cout << "Not enough points!" << endl;
              delete t;
            }
            gameMap.displayMap();
          } else if (choice == 5) {
            // Delete a Tower

            for (Tower *t : gameTowers) {
              cout << "Tower ID" << t->getId() << " Located on cell ("
                   << t->getX() << ", " << t->getY() << "): i" << endl;
            }

            int delete_choice;

            cout << "Enter the ID of the tower that you want deleted: ";
            cin >> delete_choice;

            for (Tower *t : gameTowers) {
              if (t->getId() == delete_choice) {
                // we need to delete the tower and get the refund value.
                points += t->getResale();

                // We delelete the tower.
                auto it = std::find(gameTowers.begin(), gameTowers.end(), t);
                if (it != gameTowers.end()) {
                  delete *it;
                  gameTowers.erase(it);
                  break;
                }
              }
            }

          } else if (choice == 6) {

            for (Tower *t : gameTowers) {
              cout << "Tower ID: " << t->getId()
                   << "\t has level: " << t->getLevel()
                   << "\t Tower level up will cost: " << t->getCost() << endl;
            }

            int level_up_choice;

            cout << "Enter the Id of the tower you want to level up: ";
            cin >> level_up_choice;

            for (Tower *t : gameTowers) {
              if (t->getId() == level_up_choice) {
                if (points >= t->getLevelUpCost()) {
                  points -= t->getLevelUpCost();
                  t->levelUp();
                  cout << "Leveled up Tower " << t->getId()
                       << " points left: " << points << endl;
                  break;
                }
              }
            }

          } else {
            break;
          }
        }
      }

      // Process movement for each critter.
      for (Critter *c : gameCritters) {
        c->incrementAccumulator();
        if (c->getAccumulator() == c->getSpeed()) {
          c->setAccumulator(0);
          if (c->getX() == gameMap.getExitPoint().first &&
              c->getY() == gameMap.getExitPoint().second) {
            if (playerHealth >= 0) {
              playerHealth -= c->getStrength();
              cout << "Player has " << playerHealth << endl;
              if (playerHealth <= 0) {
                cout << "PLAYER is DEAD!! GAME OVER" << endl;
                return 0;
              }
            }
          }
          c->move(&gameMap);
        }
      }

      // Process tower actions: attack critters in range.
      for (Tower *t : gameTowers) {
        t->increment_accumulator();
        if (t->getAccumulator() == t->getFireRate()) {
          t->setAccumulator(0);
          Critter *c = t->inRange(gameCritters);
          if (c != nullptr) {
            t->attack(c);
            if (c->isDead()) {
              points += c->getValue();
              cout << "Player has gained " << c->getValue()
                   << " points now player has " << points << endl;
              auto it = std::find(gameCritters.begin(), gameCritters.end(), c);
              if (it != gameCritters.end()) {
                delete *it;
                gameCritters.erase(it);
              }
            }
          }
        }
      }

      // Update and display the map after each tick.
      gameMap.displayMap();
    }
  } else {
    cout << "Map is invalid!" << endl;
  }
  // -------------------------------------------------------------------------------------------

  return 0;
}
