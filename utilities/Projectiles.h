#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "raylib.h"
// This is the struct that will be used to define projectiles in the game.
struct Projectile {
  Vector2 pos;
  Vector2 velocitity;
  Texture2D texture;
  float radius;
};

#endif // !PROJECTILES_H
