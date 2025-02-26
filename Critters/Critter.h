#ifndef CRITTER_H
#define CRITTER_H

#include "../Maps/Map.h"
#include "raylib.h"
#include <vector>

// Base Critter class for continuous movement.
class Critter {
public:
  int health;         // Health points.
  float speed;        // Movement speed in pixels per second.
  int attackStrength; // Attack strength.

  // Current pixel position.
  Vector2 position;
  // Path: a series of grid positions (x = column, y = row) that will be
  // converted to pixel centers.
  std::vector<Vector2> path;
  int currentPathIndex; // Index of the current target in the path.

  // Pointer to the Map (to access tile size, etc.)
  Map *gameMap;

  // Constructor: starts at the given grid cell.
  Critter(Map *map, int health, float speed, int attackStrength, int startRow,
          int startCol);
  virtual ~Critter();

  // Set the path for the critter. The path is given in grid coordinates.
  void SetPath(const std::vector<Vector2> &gridPath);

  // Update position using continuous movement.
  virtual void Update(float deltaTime);

  // Draw the critter.
  virtual void Draw();
};

// Derived Critter classes.
class Squirrel : public Critter {
public:
  Squirrel(Map *map, int startRow, int startCol);
  void Update(float deltaTime) override;
  void Draw() override;
};

class Bear : public Critter {
public:
  Bear(Map *map, int startRow, int startCol);
  void Update(float deltaTime) override;
  void Draw() override;
};

class Wolf : public Critter {
public:
  Wolf(Map *map, int startRow, int startCol);
  void Update(float deltaTime) override;
  void Draw() override;
};

#endif // CRITTER_H
