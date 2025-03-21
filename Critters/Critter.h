#ifndef CRITTER_H
#define CRITTER_H

#include "../Observer/Subject.h"
#include <algorithm>
#include <utility>
#include <vector>
class Map;
enum CritterType { SQUIRREL, WOLF, BEAR };
class Critter : public Subject {
public:
  Critter(int hlt_val, int spd_val, int str_val, int row_val, int col_val,
          CritterType type_val, std::vector<std::pair<int, int>> path_val,
          int val);

  virtual ~Critter() {}

  CritterType getType();
  int getRow();
  int getCol();
  int getSpeed();
  int getStr();
  int getHealth();
  int getCID();
  int getValue();

  int getPathIndex();

  void setRow(int x);
  void setCol(int y);
  void setSpeed(int x);
  void setStr(int s);
  void setHealth(int h);
  void setPath(std::vector<std::pair<int, int>> path_val);

  std::vector<std::pair<int, int>> getPath();

  // Updates the position of the critter on the map
  void Update(Map &map,
              int tick_count); // This will move the position of the creature.

  void move(Map &map, int row, int col);

private:
  int health;
  int speed;
  int strength;
  int row;
  int col;
  int cid;
  static int nextCID;
  std::vector<std::pair<int, int>> path;
  CritterType type;
  int pathIndex;
  int value; // The value of killing the creature.
};

// We create 3 subclasses of critter that have different types and different
// values for the speed attack and strength values.
class Squirrel : public Critter {

public:
  Squirrel(std::vector<std::pair<int, int>> path)
      : Critter(5, 2, 2, -1, -1, SQUIRREL, path, 50) {}
};

class Wolf : public Critter {
public:
  Wolf(std::vector<std::pair<int, int>> path)
      : Critter(8, 3, 4, -1, -1, WOLF, path, 75) {}
};

class Bear : public Critter {
public:
  Bear(std::vector<std::pair<int, int>> path)
      : Critter(12, 5, 5, -1, -1, BEAR, path, 100) {}
};

class CritterGenerator {
private:
  int level;

public:
  CritterGenerator();

  std::vector<Critter *> critters;
  bool isEmpty();
  Critter *getCritter();
  void levelUp(std::vector<std::pair<int, int>> path);
  void generateCritters(std::vector<std::pair<int, int>> path);
};

#endif // !CRITTER_H
