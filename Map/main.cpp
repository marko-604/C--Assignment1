#include "../Critter/critter.h"
#include "../towers/towers.h"
#include "Map.h"
#include <cstddef>
#include <iostream>

using namespace std;

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
  cout << "Set path using coordinates (enter Z -1 to stop):\n";
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

    // TODO create towers.
    cout << "Enter coordinates where you want to place a tower (Enter "
            "-1 to exit): ";
    int x_val, y_val;
    cin >> x_val >> y_val;

    Tower *t1;
    if (gameMap.isValidCoordinate(x_val, y_val) &&
        gameMap.isSceneryCell(x_val, y_val)) {
      t1 = new Tower(x_val, y_val);
      gameMap.setCellToOccupied(x_val, y_val);
      t1->showTowerInfo();
      gameMap.displayMap();
    } else {
      t1 = nullptr;
      cerr << "Cannot place tower in non-secnery cell" << endl;
    }

    cout << "Enter coordinates where you want to place a Sniper Tower: ";
    cin >> x_val >> y_val;

    SniperTower *t2;
    if (gameMap.isValidCoordinate(x_val, y_val) &&
        gameMap.isSceneryCell(x_val, y_val)) {
      t2 = new SniperTower(x_val, y_val);
      gameMap.setCellToOccupied(x_val, y_val);
      t2->showTowerInfo();
      gameMap.displayMap();
    } else {
      t2 = nullptr;
      cerr << "Cannot place tower in non-secnery cell" << endl;
    }

    BombTower *t3;
    cout << "Enter coordinates where you want to place a Bomb Tower: ";
    cin >> x_val >> y_val;
    if (gameMap.isValidCoordinate(x_val, y_val) &&
        gameMap.isSceneryCell(x_val, y_val)) {
      t3 = new BombTower(x_val, y_val);
      gameMap.setCellToOccupied(x_val, y_val);
      t3->showTowerInfo();
      gameMap.displayMap();
    } else {
      t3 = nullptr;
      cerr << "Cannot place tower in non-secnery cell" << endl;
    }

    cout << "Enter coordinates where you want to place a Freezing Tower: ";
    cin >> x_val >> y_val;

    FreezingTower *t4;
    if (gameMap.isValidCoordinate(x_val, y_val) &&
        gameMap.isSceneryCell(x_val, y_val)) {
      t4 = new FreezingTower(x_val, y_val);
      gameMap.setCellToOccupied(x_val, y_val);
      t4->showTowerInfo();
      gameMap.displayMap();
    } else {
      t4 = nullptr;
      cerr << "Cannot place tower in non-secnery cell" << endl;
    }

    // TODO stuff with the pointer.

    if (t4 != nullptr) {
      delete t4;
    }

    if (t3 != nullptr) {
      delete t3;
    }

    if (t2 != nullptr) {
      delete t2;
    }

    if (t1 != nullptr) {
      delete t1;
    }

    // TODO do stuff with the towers to show how they work.
    // Need for there to be a critter in range to show how the attacking works
    // and how the tower will acquire a target.

    //  TODO create critters and test them.

  } else {
    cout << "Map is invalid!" << endl;
  }
  // -------------------------------------------------------------------------------------------

  return 0;
}
