#include "Map.h"
#include <iostream>

// Forward declaration of RunGame
void RunGame(const Map &map);

int main() {
  int gridWidth, gridHeight;
  std::cout << "Enter grid width (in tiles): ";
  std::cin >> gridWidth;
  std::cout << "Enter grid height (in tiles): ";
  std::cin >> gridHeight;
  int tileSize = 80;

  // Create a Map instance.
  Map map(gridWidth, gridHeight, tileSize);

  // Run the editor.
  bool validMap = map.RunEditor();

  if (validMap) {
    // Now run the game using the map created.
    RunGame(map);
  } else {
    std::cout << "Map creation was cancelled or invalid.\n";
  }
  return 0;
}

#include "GameMaster.h"

int main() {
  // 1) Create a game master with a 10x6 map, tileSize=32
  GameMaster gm(10, 6, 32);

  // 2) Let the user draw the map (path, entry, exit)
  if (!gm.SetupMap()) {
    // If user canceled or invalid path, quit
    return 0;
  }

  // 3) Spawn a tower at row=2, col=2
  gm.SpawnTower(REGULAR, 2, 2);

  // 4) Spawn a couple of critters
  gm.SpawnCritter(SQUIRREL, 5, 0);
  gm.SpawnCritter(WOLF, 5, 1);

  // 5) Launch the main loop
  gm.RunGameLoop();

  return 0;
}
