#pragma once

#include "Tmx.h"
#include "Math.h"
#include "Model3d.h"

class GameObject;
class Object3d;

/**
 * Internal class to store the processed data from the tmx file, along with geometry info
 */
struct Tile {
  float x = 0;
  float y = 0;
  bool exists = false;
  bool is_collidable = false;
  Object3dHolder object3d;
  void render() { object3d.render(); }
};

/**
 * Contains tile information, player and enemy positions
 * and geometric data
 */
class Map {
public:
  Map() = default;
  bool load(const std::string& file);
  std::string serialize();
  glm::vec2 getPlayerInitialPosition();
  std::vector<glm::vec2> getEnemiesInitialPositions();
  void render();
  bool isCollision(GameObject*);

private:
  bool getInitialPosition(std::string name, glm::vec2* position);  // return true if found
  Tmx::Map tmx_map_;
  float units_per_tile_ = 1.0;
  std::vector<std::vector<Tile>> tile_map_;
};