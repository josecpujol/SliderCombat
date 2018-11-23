#pragma once

#include "Tmx.h"
#include "mathnphysics/Math.h"
#include "graphics/Model3d.h"
#include "mathnphysics/Collision.h"
#include "Pose.h"

class GameObject;
class Object3d;

/**
 * Internal class to store the processed data from the tmx file, along with geometry info
 */
struct Tile {
  bool exists = false;
  bool isCollidable() const {
    return collision_area.getType() != CollisionAreaType::None;
  }
  CollisionArea collision_area;
  Object3dHolder object3d;
  void render() { object3d.render(false); }
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
  Pose getPlayerInitialPose();
  std::vector<Pose> getInitialPoses(const std::string&);
  void render();
  void renderCollisionArea();
  bool isCollision(const CollisionArea& collision_area, glm::vec2* collision_point, glm::vec2* normal);

private:
  Tmx::Map tmx_map_;
  float units_per_tile_ = 1.0;
  std::vector<std::vector<Tile>> tile_map_;
};
