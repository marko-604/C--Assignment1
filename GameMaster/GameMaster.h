#ifndef GAME_MASTER_H
#define GAME_MASTER_H
// We will include all of the files that we have created for Critter, Towers and
// Map then we will provide all of the functions and interface necessary to play
// the game.

// #include "../Towers/Tower.h"
// #include"../Critters/Critter.h"
#include "../Maps/Map.h"
#include <iostream>
#include <vector>

// The parts of the game.
Map & game_map;
std::vector<Critter *> critters;
std::vector<Tower *> towers;
int playerHealth;
int playerPoints;

void createMap();
void initializePlayer(int health, int points);

void RunGame(const Map &map);
#endif // !GAME_MASTER_H
