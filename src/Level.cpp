#include "Level.h"

#include <thread>
#include <algorithm>

#include "graphics/OpenGlResources.h"

#include "ResourcesManager.h"
#include "utils/Logger.h"
#include "gameobjects/Projectile.h"
#include "gameobjects/Explosion.h"
#include "gameobjects/PowerUps.h"
#include "graphics/OpenGlState.h"
#include "Map.h"

Level::Level(Map* map) : map_(map) {
  local_player_ = std::make_shared<SliderLocalPlayer>(map->getPlayerInitialPose());
  // Add objects to the level
  objects_.push_back(local_player_);
  auto enemy_poses = map->getInitialPoses("Enemy");
  for (auto &enemy_pose : enemy_poses) {
    objects_.push_back(std::make_shared<SliderComputerEnemy>(enemy_pose));
  }
  
  auto health_poses = map->getInitialPoses("Health");
  for (auto &health_pose : health_poses) {
    objects_.push_back(std::make_shared<HealthPowerUp>(glm::vec2(health_pose.position)));
  }


  hud_ = std::make_unique<Hud>(this);

  camera_.lookAt(glm::vec3(0, 0, 10), glm::vec3(30, 30, 0), glm::vec3(1, 1, 0));


  // TODO: unregister?
  Event::manager.subscribeToRemoveObjectEvent([this](GameObject* object) {
    this->removeObject(object);
  });

  Event::manager.subscribeToAddObjectEvent([this](GameObject* object) {
    this->addObject(object);
  });
}

void Level::addObject(GameObject* object) {
  objects_to_add_.push_back(std::shared_ptr<GameObject>(object));
}
void Level::removeObject(GameObject* object) {
  objects_to_remove_.push_back(object);
}

void Level::updateRenderFlags(const Uint8* keys) {
  uint8_t count = ((render_collision_area_) * 2) + (render_objects_);
  if (keys[SDL_SCANCODE_0] && Clock::now() > last_time_render_state_changed_ + 500ms) {
    count = (count + 1) % 4;
    last_time_render_state_changed_ = Clock::now();
  }
  render_collision_area_ = count >> 1;
  render_objects_ = count & 0x01;
}

void Level::updateCameraSetup(const Uint8* keys) {
  if (keys[SDL_SCANCODE_1]) {
    camera_.lookAt(glm::vec3(44, 30, 120), glm::vec3(44, 44, 0), glm::vec3(0, 0, 1));
  }
  if (keys[SDL_SCANCODE_2]) {
    camera_.follow(local_player_.get(), glm::vec3(-5, 0, 4), glm::vec3(2, 0, 2));  // Follow from behind
  }
  if (keys[SDL_SCANCODE_3]) {
    camera_.follow(local_player_.get(), glm::vec3(0.2, 0, 1), glm::vec3(2, 0, 1));  // subjective view
  }
  if (keys[SDL_SCANCODE_4]) {
    camera_.follow(local_player_.get(), glm::vec3(-1, 0, 50), glm::vec3(10, 0, 0));  // follow from top
  }
  if (keys[SDL_SCANCODE_5]) {
    camera_.testMode();  // follow from top
  }
}

void Level::update(uint32_t elapsed_us) {
  const Uint8* keys = SDL_GetKeyboardState(nullptr);
  // Pause!!!
  if (keys[SDL_SCANCODE_P]) {
    return;
  }

  updateCameraSetup(keys);
  updateRenderFlags(keys);

  addPendingObjects();

  for (auto obj : objects_) {
    obj->update(elapsed_us);
  }

  checkCollisions();

  removePendingObjects();
}

void Level::addPendingObjects() {
  for (auto obj : objects_to_add_) {
    objects_.push_back(obj);
  }
  objects_to_add_.clear();
}

void Level::removePendingObjects() {
  for (auto object_to_remove : objects_to_remove_) {
    auto i = std::begin(objects_);

    auto size_objects_before = objects_.size();
    while (i != std::end(objects_)) {
      if (i->get() == object_to_remove) {
        i = objects_.erase(i);
      } else {
        ++i;
      }
    }
  }
  objects_to_remove_.clear();
}

void Level::checkCollisions() {

  // Collisions of objects with map
  for (auto obj : objects_) {
    if (!obj->canCollide()) {
      continue;
    }
    glm::vec2 collision_point;
    glm::vec2 normal;
    if (map_->isCollision(obj->getCollisionArea(), &collision_point, &normal)) {
      obj->onCollision(nullptr, collision_point, &normal);

      // Debug
      VectorWithTimer vector;
      vector.bound_vector.origin = collision_point;
      vector.bound_vector.direction = normal;

      vector.expiration = Clock::now() + 2s;
      collision_points_.push_back(vector);
    }
  }

  // Objects with objects
  size_t objects_size = objects_.size();
  for (int i = 0; i < (objects_size - 1); i++) {
    GameObject* obj1 = (&objects_[i])->get();
    if (!obj1->canCollide()) {
      continue;
    }
    for (int j = i + 1; j < objects_size; j++) {
      GameObject* obj2 = (&objects_[j])->get();
      if (!obj2->canCollide()) {
        continue;
      }
      glm::vec2 collision_point;


      if (Collision::isCollision(obj1->getCollisionArea(), obj2->getCollisionArea(), &collision_point)) {
        // Debug
        VectorWithTimer collision_info;
        collision_info.bound_vector.origin = collision_point;
        collision_info.bound_vector.direction = glm::vec2(obj1->getDisplacement());
        collision_info.expiration = Clock::now() + 2s;
        collision_points_.push_back(collision_info);

        obj1->onCollision(obj2, collision_point, nullptr);
        obj2->onCollision(obj1, collision_point, nullptr);  // TODO: -
      }
    }
  }
}

void Level::drawSkyDome() {
  int segments = 30;
  int num_vertices = (segments + 1) * 2;

  glm::vec3 color_top = glm::vec3(1.f, 1.f, 1.f);
  glm::vec3 color_bottom = glm::vec3(137.f / 255.f, 165.f / 255.f, 237.f / 255.f);

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> colors;
  vertices.reserve(num_vertices);
  colors.reserve(num_vertices);

  // Draw cilinder
  float radius = 1.f;
  float top = 1.f;
  float bottom = -0.05f;

  for (int i = 0, index = 0; i <= segments; i++, index++) {
    float x = sin((6.28318f * i) / segments);
    float y = cos((6.28318f * i) / segments);
    colors.push_back(color_bottom);
    vertices.push_back(glm::vec3(x, y, bottom));
    colors.push_back(color_top);
    vertices.push_back(glm::vec3(x, y, top));
  }

  glDepthMask(false);
  glDisable(GL_DEPTH_TEST);

  // TODO: have this as an object. Preallocate mesh
  OpenGlResources::drawMesh(vertices, colors, GL_TRIANGLE_STRIP);
 
  glEnable(GL_DEPTH_TEST);
  glDepthMask(true);
}

void Level::render() {
  int width = 0;
  int height = 0;
  ResourcesManager::getInstance().getWindowDimensions(&width, &height);
  glClearColor(83.f/255.f, 134.f/255.f, 1.f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_NORMALIZE);

  GLfloat ratio = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  OpenGlState::getInstance().matrixMode(MatrixMode::kProjection);
  glm::mat4 perspective = glm::perspective(45.0f, ratio, 0.2f, 130.0f);
  OpenGlState::getInstance().loadMatrix(perspective);

  OpenGlState::getInstance().matrixMode(MatrixMode::kModelView);
  OpenGlState::getInstance().loadIdentity();
  camera_.applyRotationOnly();
  drawSkyDome();

  OpenGlState::getInstance().loadIdentity();
  camera_.apply();

  // Lights
  scene_.setAmbientLight(glm::vec3(0.25f));
  scene_.setDiffuseLight(glm::vec4(0.f, 0.0f, 1.f, 0.f), glm::vec3(0.25f));

  OpenGlResources::drawAxis();
  /*
  for (auto &point : collision_points_) {
    glPushMatrix();
    glTranslatef(point.bound_vector.origin.x, point.bound_vector.origin.y, 0);
    OpenGlResources::drawCircle(0.5, 10);
    glPopMatrix();
    OpenGlResources::drawVector(point.bound_vector.origin, point.bound_vector.origin + point.bound_vector.direction, glm::vec3(1, 1, 1));
  }*/
  collision_points_.erase(std::remove_if(collision_points_.begin(), collision_points_.end(),
    [](const VectorWithTimer& o) {return o.expiration < Clock::now(); }), collision_points_.end());

  if (render_objects_) map_->render(scene_);
  if (render_collision_area_) map_->renderCollisionArea();

  // Draw objects
  for (auto obj : objects_) {
    if (render_collision_area_) obj->renderCollisionArea();
    if (render_objects_) obj->render(scene_);
  }

  // Draw overlay: health, status
  hud_->display();
}
