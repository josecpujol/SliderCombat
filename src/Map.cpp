#include "Map.h"

#include <sstream>
#include <GL/glew.h>

#include "Logger.h"
#include "GameObject.h"

constexpr char kUnitsPerTile[] = "unitspertile";
constexpr char kCollidableProperty[] = "collidable";



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
 /* collision_map_.resize(tile_layer->GetWidth());
  for (int x = 0; x < tile_layer->GetWidth(); x++) {
    collision_map_[i].resize(tile_layer->GetHeight());
  }

  
 
  for (int x = 0; x < tile_layer->GetWidth(); x++) {
    for (int y = 0; y < tile_layer->GetHeight(); y++) {
      if (tile_layer->GetTileTilesetIndex(x, y) == -1) {
        continue;
      }
      const Tmx::MapTile& map_tile = tile_layer->GetTile(x, y);
      const Tmx::Tileset* tileset = tmx_map_.GetTileset(map_tile.tilesetId);

      collision_map_[x][y].exists = true;
    }
  }
  */
  
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

  glPushMatrix();
  float z = 3;
  glm::vec2 p0;
  glm::vec2 p1;
  const Tmx::TileLayer *tile_layer = tmx_map_.GetTileLayer(0);

  for (int x = 0; x < tile_layer->GetWidth(); x++) {
    for (int y = 0; y < tile_layer->GetHeight(); y++) {
      if (tile_layer->GetTileTilesetIndex(x, y) == -1) {
        continue;
      }
   //   LOG_DEBUG("Tile info: " << x << " " << y << " " << tile_layer->GetTile(x, y).id);
      switch (tile_layer->GetTile(x, y).id) {
      case 0:  // Normal
        glBegin(GL_QUADS);

        glColor3f((x + y) % 2 ? 0.5 : 0, 0.0, 0.0);
        glVertex2f(x * units_per_tile_, y * units_per_tile_);
        glVertex2f(x * units_per_tile_, (y + 1) * units_per_tile_);
        glVertex2f((x + 1) * units_per_tile_, (y + 1) * units_per_tile_);
        glVertex2f((x + 1) * units_per_tile_, y * units_per_tile_);
        glEnd();

        break;
      case 8:  // Wall
        glBegin(GL_QUAD_STRIP);

        glColor3f(0.5, 0.5, 0.5);
        p0 = glm::vec2(x * units_per_tile_, y * units_per_tile_);
        p1 = glm::vec2((x + 1) * units_per_tile_, (y + 1) * units_per_tile_);
        glVertex2f(p0.x, p0.y); glVertex3f(p0.x, p0.y, z); 
        glVertex2f(p0.x, p1.y); glVertex3f(p0.x, p1.y, z);
        glVertex2f(p1.x, p1.y); glVertex3f(p1.x, p1.y, z);
        glVertex2f(p1.x, p0.y); glVertex3f(p1.x, p0.y, z);
        glEnd();

        glBegin(GL_QUADS);

        glVertex3f(p0.x, p0.y, z);
        glVertex3f(p0.x, p1.y, z);
        glVertex3f(p1.x, p1.y, z);
        glVertex3f(p1.x, p0.y, z);
        glEnd();


        break;
      default:
        assert(false);
      }
    }
  }
  glPopMatrix();
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