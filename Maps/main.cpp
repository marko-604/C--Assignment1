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
