#include "Map.h"

#include <sstream>

#include "utils/Logger.h"
#include "gameobjects/GameObject.h"
#include "Model3d.h"
#include "ResourcesManager.h"
#include "OpenGlResources.h"

constexpr char kUnitsPerTile[] = "unitspertile";
constexpr char kCollidableProperty[] = "collidable";

float getRotation(const Tmx::MapTile& map_tile) {
  if (!map_tile.flippedVertically && !map_tile.flippedHorizontally && !map_tile.flippedDiagonally) return 0;
  if ( map_tile.flippedVertically && !map_tile.flippedHorizontally &&  map_tile.flippedDiagonally) return 90;
  if ( map_tile.flippedVertically &&  map_tile.flippedHorizontally && !map_tile.flippedDiagonally) return 180;
  if (!map_tile.flippedVertically &&  map_tile.flippedHorizontally &&  map_tile.flippedDiagonally) return -90;
  LOG_ERROR("Tiles with rotation only are supported. Map may not be drawn correctly");
  return 0;
}

bool Map::load(const std::string& file) {
  tmx_map_.ParseFile(file);
  if (tmx_map_.HasError()) {
    return false;
  }

  LOG_DEBUG(serialize());
  
  // Sanity check
  auto properties = tmx_map_.GetProperties();
  bool is_units_per_tile_found = false;
  if (!properties.HasProperty(std::string(kUnitsPerTile))) {
    LOG_ERROR("No " << kUnitsPerTile << " property");
    return false;
  }

  units_per_tile_ = properties.GetFloatProperty(kUnitsPerTile, 1.0);

  size_t num_tile_layers = tmx_map_.GetNumTileLayers();
  if (num_tile_layers != 1) {
    LOG_ERROR("# tile layers different from 1, " << num_tile_layers);
    return false;
  }

  // Create geometry
  const Tmx::TileLayer* tile_layer = tmx_map_.GetTileLayer(0);
  tile_map_.resize(tile_layer->GetWidth());
  for (int x = 0; x < tile_layer->GetWidth(); x++) {
    tile_map_[x].resize(tile_layer->GetHeight());
  }

  Model3d* model3d = ResourcesManager::getInstance().getModel3d(ModelType::kTiles);
  if (!model3d) return false;

  for (int x = 0; x < tile_layer->GetWidth(); x++) {
    for (int y = 0; y < tile_layer->GetHeight(); y++) {
      if (tile_layer->GetTileTilesetIndex(x, y) == -1) {
        continue;
      }
      const Tmx::MapTile& map_tile = tile_layer->GetTile(x, y);
      const Tmx::Tileset* tileset = tmx_map_.GetTileset(map_tile.tilesetId);

      Tile* gametile = &(tile_map_[x][y]);
      gametile->exists = true;

      std::string object_name = std::string("tile") + std::to_string(map_tile.id);
      Object3d* obj3d = model3d->getObject3d(object_name);

      if (!obj3d) {
        LOG_ERROR("Could not find tile model for tile " << map_tile.id);
        return false;
      }
      gametile->object3d.setObject3d(obj3d);
      // Tiles are -0.5 -> 0.5 in the model. We need to move them to 0 -> 1
      gametile->object3d.setTranslation(units_per_tile_ * glm::vec3(x + 0.5, y + 0.5, 0)); 
      gametile->object3d.setScale(glm::vec3(units_per_tile_, -units_per_tile_, units_per_tile_));
      float angle = getRotation(map_tile);
      gametile->object3d.setRotationZ(angle);
      
      const Tmx::Tile* tile = tileset->GetTile(map_tile.id);
      if (tile) {
        Tmx::PropertySet properties = tile->GetProperties();
        if (properties.GetBoolProperty(std::string(kCollidableProperty), false)) {
          Rectangle rect(
            glm::vec2(x * units_per_tile_, y * units_per_tile_) + glm::vec2(units_per_tile_ / 2),
            glm::vec2(units_per_tile_, units_per_tile_)
          );
          gametile->collision_area.setCollisionPrimivite(rect);
        }
      }
    }
  }

  
  return true;
}

std::vector<Pose> Map::getInitialPoses(const std::string& name) {
  std::vector<Pose> poses;
  for (auto& object_group : tmx_map_.GetObjectGroups()) {
    if (object_group->GetName() != "StartPositions") {
      continue;
    }
    for (auto& object : object_group->GetObjects()) {
      if (object->GetName() == name) {
        glm::vec3 position{
          (object->GetX() * units_per_tile_) / tmx_map_.GetTileWidth(),
          (object->GetY() * units_per_tile_) / tmx_map_.GetTileHeight(),
          0.f};
        float rotation = (float)object->GetRot();
        poses.push_back(Pose{position, rotation});
      }
    }
  }
  return poses;
}


Pose Map::getPlayerInitialPose() {
  std::vector<Pose> poses = getInitialPoses(std::string("Player"));
  if (poses.size() != 1) {
    LOG_ERROR("Could not find player initial position");
    return Pose{glm::vec3(0.f), 0.f};
  }
  return poses[0];
}

bool Map::isCollision(const CollisionArea& collision_area, glm::vec2* collision_point, glm::vec2* normal) {
  glm::vec2 pos = glm::vec2(collision_area.getPosition());
  int tile_x = pos.x / units_per_tile_;
  int tile_y = pos.y / units_per_tile_;


  for (int x = tile_x - 1; x <= tile_x + 1; x++) {
    if (x < 0 || x >= tile_map_.size()) continue;
    for (int y = tile_y - 1; y <= tile_y + 1; y++) {
      if (y < 0 || y >= tile_map_[x].size()) continue;
      Tile* tile = &(tile_map_[x][y]);
      if (!tile->exists) continue;
      bool is_collision = Collision::isCollision(collision_area, tile->collision_area, collision_point);
      if (is_collision) {
        // Compute normal: tile based: rectangle, no rotations
        glm::vec2 v = *collision_point - tile->collision_area.getPosition();
        if (abs(v.x) > abs(v.y)) {
          if (v.x > 0) *normal = glm::vec2(1, 0);
          else *normal = glm::vec2(-1, 0);
        } else {
          if (v.y > 0) *normal = glm::vec2(0, 1);
          else *normal = glm::vec2(0, -1);
        }
        return true;
      }
    }
  }
  return false;
}

void Map::render() {
  for (int x = 0; x < tile_map_.size(); x++) {
    for (int y = 0; y < tile_map_[x].size(); y++) {
      Tile* gametile = &(tile_map_[x][y]);
      if (!gametile->exists) {
        continue;
      }
      gametile->render();
    }
  }
}

void Map::renderCollisionArea() {
  for (int x = 0; x < tile_map_.size(); x++) {
    for (int y = 0; y < tile_map_[x].size(); y++) {
      Tile* gametile = &(tile_map_[x][y]);
      if (!gametile->exists) {
        continue;
      }
      if (gametile->isCollidable()) {
        gametile->collision_area.render();
      }
    }
  }
}


std::string Map::serialize() {
  std::stringstream ss;
  ss << "Version:  " << tmx_map_.GetVersion() << std::endl;
  ss << "Orientation: " << tmx_map_.GetOrientation() << std::endl;
  ss << "Width: " << tmx_map_.GetWidth() << std::endl;
  ss << "Height: " << tmx_map_.GetHeight() << std::endl;
  ss << "Tile Width: " << tmx_map_.GetTileWidth() << std::endl;
  ss << "Tile Height: " << tmx_map_.GetTileHeight() << std::endl;
  for (int i = 0; i < tmx_map_.GetNumTileLayers(); ++i) {
    const Tmx::TileLayer *tile_layer = tmx_map_.GetTileLayer(i);
    ss << "Layer  " << i << ": " << tile_layer->GetName().c_str() << std::endl;
  }
  return ss.str();
}