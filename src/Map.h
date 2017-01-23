#pragma once

#include "Tmx.h"
#include "Math.h"

class GameObject;

/**
 * Internal class to store the processed data from the tmx file, along with geometry info
 */
struct Tile {
  bool exists = false;
  bool is_collidable = false;
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
  glm::vec2 getPlayerInitialPosition() const;
  std::vector<glm::vec2> getEnemiesInitialPositions();
  void render();
  bool isCollision(GameObject*);

private:
  Tmx::Map tmx_map_;
  float units_per_tile_ = 1.0;
  std::vector<std::vector<Tile>> collision_map_;
};