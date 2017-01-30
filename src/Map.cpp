#include "Map.h"

#include <sstream>
#include <GL/glew.h>

#include "Logger.h"
#include "GameObject.h"
#include "Model3d.h"
#include "ResourcesManager.h"

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

  int num_tile_layers = tmx_map_.GetNumTileLayers();
  if (num_tile_layers != 1) {
    LOG_ERROR("# tile layers different from 1, " << num_tile_layers);
    return false;
  }

  // Create geometry: a buffer per type of tile
  // TODO
   
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
      gametile->x = x * units_per_tile_;
      gametile->y = y * units_per_tile_;
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
        gametile->is_collidable = properties.GetBoolProperty(std::string(kCollidableProperty, false));
      }
    }
  }

  
  return true;
}

glm::vec2 Map::getPlayerInitialPosition() const {
  return glm::vec2(20.0, 20.0);
}

std::vector<glm::vec2> Map::getEnemiesInitialPositions() {
  return {glm::vec2(40.0, 20.0), glm::vec2(40.0, 40.0)};
}

bool Map::isCollision(GameObject* obj) {
  // TODO: do per tile
  glm::vec2 pos = glm::vec2(obj->getPosition());
  if (pos.x < 0 || 
    pos.y < 0 || 
    pos.x > tmx_map_.GetWidth() * units_per_tile_ || 
    pos.y > tmx_map_.GetHeight() * units_per_tile_) {
    return true;
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