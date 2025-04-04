#ifndef MAP_H
#define MAP_H

#include "../Observer/Subject.h"
#include "raylib.h"
#include <utility>
#include <vector>

class Critter;
class Tower;

/// @brief Enumeration representing all possible tile types on the grid.
enum TileType {
  EMPTY,           ///< An empty tile.
  PATH,            ///< A path tile (for critter movement).
  ENTRY,           ///< Entry point for critters.
  EXIT,            ///< Exit point for critters.
  FREEZINGTOWER,   ///< A freezing tower is placed here.
  SNIPERTOWER,     ///< A sniper tower is placed here.
  BOMBTOWER,       ///< A bomb tower is placed here.
  REGULARTOWER,    ///< A regular tower is placed here.
  WOLFCRITTER,     ///< A wolf-type critter is occupying this tile.
  SQUIRRELCRITTER, ///< A squirrel-type critter is occupying this tile.
  BEARCRITTER      ///< A bear-type critter is occupying this tile.
};

/// @brief Represents the game map and its grid, including entry/exit points and towers.
class Map : public Subject {
public:
  int gridWidth;    ///< Width of the grid in number of tiles.
  int gridHeight;   ///< Height of the grid in number of tiles.
  int tileSize;     ///< Size of each tile in pixels.

  std::vector<std::vector<TileType>> grid; ///< 2D grid representing tile types.

  int entryRow; ///< Row coordinate of the entry point.
  int entryCol; ///< Column coordinate of the entry point.
  int exitRow;  ///< Row coordinate of the exit point.
  int exitCol;  ///< Column coordinate of the exit point.

  /**
   * @brief Constructor to initialize the map.
   * 
   * @param width Width of the map grid.
   * @param height Height of the map grid.
   * @param tileSize Size of each tile (in pixels).
   */
  Map(int width, int height, int tileSize);

  /// @brief Destructor for cleanup.
  ~Map();

  /**
   * @brief Get the path from entry to exit.
   * 
   * @return std::vector<std::pair<int, int>> Vector of (row, col) coordinates.
   */
  std::vector<std::pair<int, int>> getPath();

  /// @brief Draws the map grid and editor instructions on the screen.
  void Draw();

  /**
   * @brief Set a specific tile to PATH type.
   * 
   * @param row Row index.
   * @param col Column index.
   */
  void setToPath(int row, int col);

  /**
   * @brief Toggle the presence of a tower on the given tile.
   * 
   * @param tower Pointer to the tower.
   * @param row Row index.
   * @param col Column index.
   */
  void ToggleTower(Tower *tower, int row, int col);

  /**
   * @brief Toggle the presence of a critter on the given tile.
   * 
   * @param critter Pointer to the critter.
   * @param row Row index.
   * @param col Column index.
   */
  void ToggleCritter(Critter *critter, int row, int col);

  /**
   * @brief Place a critter on a specific tile.
   * 
   * @param critter Reference to the critter.
   * @param row Row index.
   * @param col Column index.
   * @return true if placement was successful.
   * @return false otherwise.
   */
  bool placeCritter(Critter &critter, int row, int col);

  /**
   * @brief Place a tower on a specific tile.
   * 
   * @param tower Reference to the tower.
   * @param row Row index.
   * @param col Column index.
   * @return true if placement was successful.
   * @return false otherwise.
   */
  bool PlaceTower(Tower &tower, int row, int col);

  /**
   * @brief Toggle a tile's type between EMPTY and PATH.
   * 
   * @param row Row index.
   * @param col Column index.
   */
  void TogglePath(int row, int col);

  /**
   * @brief Set the tile at (row, col) as the entry point.
   * 
   * @param row Row index.
   * @param col Column index.
   */
  void SetEntry(int row, int col);

  /**
   * @brief Set the tile at (row, col) as the exit point.
   * 
   * @param row Row index.
   * @param col Column index.
   */
  void SetExit(int row, int col);

  /**
   * @brief Set a tile to represent a scenery or non-interactive state.
   * 
   * @param row Row index.
   * @param col Column index.
   */
  void setToScenery(int row, int col);

  /**
   * @brief Check if the path from entry to exit is valid.
   * 
   * @return true if valid.
   * @return false otherwise.
   */
  bool IsValidPath();

  /**
   * @brief Run the map editor interface.
   * 
   * @return true if user confirms and saves a valid map.
   * @return false otherwise (e.g., user exits or cancels).
   */
  bool RunEditor();

/**
 * @brief Loads a map from a text file and updates the internal grid.
 *
 * @param filename Path to the map text file.
 * @return true if the file was successfully loaded, false otherwise.
 */
  bool LoadFromFile(const std::string &filename);

};

/**
 * @brief Function to run the game using the given map.
 * 
 * @param map Reference to a valid map.
 */
void RunGame(Map &map);


/**
 * @brief Renders a miniature preview of a map from a file at a given position.
 *
 * @param filename Path to the map file.
 * @param x X-coordinate to draw the preview.
 * @param y Y-coordinate to draw the preview.
 * @param tileSize Size of each tile in the preview.
 */
void DrawMiniMapPreview(const std::string &filename, int x, int y, int tileSize);


#endif // MAP_H
