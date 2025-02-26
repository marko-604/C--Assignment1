
#include "Critter.h"
#include <cmath>

// Helper function: Convert grid coordinates (row, col) into pixel center
// position.
static Vector2 GridToPixel(Map *map, int row, int col) {
  Vector2 pos;
  pos.x = col * map->tileSize + map->tileSize / 2.0f;
  pos.y = row * map->tileSize + map->tileSize / 2.0f;
  return pos;
}

// Base Critter constructor.
Critter::Critter(Map *map, int health, float speed, int attackStrength,
                 int startRow, int startCol)
    : gameMap(map), health(health), speed(speed),
      attackStrength(attackStrength), currentPathIndex(0) {
  // Set initial position (center of the starting tile).
  position = GridToPixel(map, startRow, startCol);
}

Critter::~Critter() {}

void Critter::SetPath(const std::vector<Vector2> &gridPath) {
  // Convert grid coordinates to pixel centers.
  path.clear();
  for (auto gridPos : gridPath) {
    int row = (int)gridPos.y;
    int col = (int)gridPos.x;
    path.push_back(GridToPixel(gameMap, row, col));
  }
  currentPathIndex = 0;
}

void Critter::Update(float deltaTime) {
  if (path.empty() || currentPathIndex >= (int)path.size())
    return;

  Vector2 target = path[currentPathIndex];
  // Calculate direction vector from current position to target.
  Vector2 dir = {target.x - position.x, target.y - position.y};
  float distance = sqrtf(dir.x * dir.x + dir.y * dir.y);

  // If close enough to the target, snap to it and advance to the next waypoint.
  if (distance < 1.0f) {
    position = target;
    currentPathIndex++;
  } else {
    // Normalize direction.
    dir.x /= distance;
    dir.y /= distance;
    // Compute movement distance (speed is in pixels per second).
    float moveDist = speed * deltaTime;
    if (moveDist > distance)
      moveDist = distance;
    // Update position.
    position.x += dir.x * moveDist;
    position.y += dir.y * moveDist;
  }
}

void Critter::Draw() {
  // Base draw: a white circle.
  DrawCircleV(position, gameMap->tileSize / 3.0f, WHITE);
}

// --- Squirrel Implementation ---
Squirrel::Squirrel(Map *map, int startRow, int startCol)
    : Critter(map, 50, 100.0f, 5, startRow,
              startCol) // 100 pixels/sec, for example.
{}

void Squirrel::Update(float deltaTime) { Critter::Update(deltaTime); }

void Squirrel::Draw() {
  DrawCircleV(position, gameMap->tileSize / 3.0f, BROWN);
}

// --- Bear Implementation ---
Bear::Bear(Map *map, int startRow, int startCol)
    : Critter(map, 150, 50.0f, 20, startRow, startCol) // Slower speed.
{}

void Bear::Update(float deltaTime) { Critter::Update(deltaTime); }

void Bear::Draw() { DrawCircleV(position, gameMap->tileSize / 2.5f, BLACK); }

// --- Wolf Implementation ---
Wolf::Wolf(Map *map, int startRow, int startCol)
    : Critter(map, 100, 75.0f, 15, startRow, startCol) // Intermediate speed.
{}

void Wolf::Update(float deltaTime) { Critter::Update(deltaTime); }

void Wolf::Draw() { DrawCircleV(position, gameMap->tileSize / 3.5f, PURPLE); }
