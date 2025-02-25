/*
 *
 *  This is a test driver for the tower classes declared in the towers.h header
 * and defined in the towers.cpp file.
 *
 *
 */

#include "towers.h"
#include <iostream>

int main() {

  Critter *c1 = new Critter(3, 4);

  // Testing creating a simple tower object.
  Tower *t1 = new Tower(3, 3);
  t1->showTowerInfo(); // Display the towers info.
  t1->attack(c1);
  t1->levelUp();
  t1->levelUp();
  t1->levelUp();
  t1->levelUp();
  t1->levelUp();
  t1->levelUp();
  delete t1;

  std::cout << "Tower deleted" << std::endl;
  ;
  std::cout << "\n\n";

  // Testing creating a SniperTower object.
  SniperTower *t2 = new SniperTower(3, 4);
  t2->showTowerInfo();
  t2->attack(c1);
  delete t2;

  std::cout << "Tower deleted" << std::endl;
  ;
  std::cout << "\n\n";

  // Testing constructor for BombTower.
  BombTower *t3 = new BombTower(4, 5);
  t3->showTowerInfo();
  delete t3;

  std::cout << "Tower deleted." << std::endl;
  std::cout << "\n\n";

  FreezingTower *t4 = new FreezingTower(4, 5);
  t4->showTowerInfo();
  t4->attack(c1);
  delete t4;

  std::cout << "Tower deleted." << std::endl;
  std::cout << "\n\n";
  return 0;
}
