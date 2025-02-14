#ifndef CRITTER_H
#define CRITTER_H

#include "../Map/Map.h"
#include <iostream>
#include <string>
#include <vector>

class Critter {
public:
  Critter(int spd_val, int hlt_val, int str_val, int x_val, int y_val)
      : speed(spd_val), health(hlt_val), strength(str_val), cid(NextCID++) {};

  int getSpeed() { return speed; }
  int getHealth() { return health; }
  int getStrength() { return strength; }
  int getCID() { return cid; }

  void setHealth(int hlt_val) { health = hlt_val; }
  void setSpeed(int spd_val) { speed = spd_val; }
  void setStrength(int str_val) { strength = str_val; }

  void displayInfo();
  int getX() { return x; }
  int getY() { return y; }

  void move(Map *gameMap); // We will use the argument of the move function to
                           // be the game map
  void checkHealth();

private:
  static int NextCID;
  int cid;
  int speed;
  int health;
  int strength;
  int x;
  int y;
};

class Squirrel : public Critter {
public:
  Squirrel(int x_val, int y_val) : Critter(4, 5, 2, x_val, y_val) {}
};

class Wolf : public Critter {
public:
  Wolf(int x_val, int y_val) : Critter(2, 10, 5, x_val, y_val) {}
};

class Bear : public Critter {
public:
  Bear(int x_val, int y_val) : Critter(1, 20, 5, x_val, y_val) {}
};

#endif
