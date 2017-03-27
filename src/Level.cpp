#include "Level.h"

#include <thread>
#include <algorithm>

#include "OpenGlResources.h"

#include "ResourcesManager.h"
#include "Logger.h"
#include "Fire.h"
#include "Map.h"

Level::Level(Map* map) : map_(map) {
  local_player_ = std::make_shared<SliderLocalPlayer>(map->getPlayerInitialPosition(), 0.0f);
  // Add objects to the level
  objects_.push_back(local_player_);
  auto enemy_positions = map->getEnemiesInitialPositions();
  for (auto &enemy_position : enemy_positions) {
    objects_.push_back(std::make_shared<SliderComputerEnemy>(enemy_position, 180.0f));
  }

  camera_.lookAt(glm::vec3(0, 0, 10), glm::vec3(30, 30, 0), glm::vec3(1, 1, 0));


  // TODO: unregister?
  Event::manager.subscribeToFireEvent([this](const FireEvent& event) {
    this->onFire(event);
  });

  Event::manager.subscribeToRemoveObjectEvent([this](GameObject* object) {
    this->removeObject(object);
  });
}

void Level::onFire(const FireEvent& event) {
  //  LOG_DEBUG("Fire!!");
  addObject(new Fire(event.pos_, event.rot_z_));
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
    camera_.lookAt(glm::vec3(0,0,10), glm::vec3(30, 30, 0), glm::vec3(1, 1, 0));
  }
  if (keys[SDL_SCANCODE_2]) {
    camera_.follow(local_player_.get(), glm::vec3(0, -5, 4), glm::vec3(0, 2, 2));  // Follow from behind
  }
  if (keys[SDL_SCANCODE_3]) {
    camera_.follow(local_player_.get(), glm::vec3(0, 0.2, 1), glm::vec3(0, 2, 1));  // subjective view
  }
  if (keys[SDL_SCANCODE_4]) {
    camera_.follow(local_player_.get(), glm::vec3(0, -1, 50), glm::vec3(0,10,0));  // follow from top
  }

}

void Level::update(const Uint8* keys, uint32_t elapsed_us) {
  // Pause!!!
  if (keys[SDL_SCANCODE_P]) {
    return;
  }
  loop_count_++;

  updateCameraSetup(keys);
  updateRenderFlags(keys);

  addPendingObjects();

  for (auto obj : objects_) {
    obj->update(keys, elapsed_us);
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
    if (objects_.size() == size_objects_before) {
      LOG_ERROR("An object was not deleted");
    }
  }
  objects_to_remove_.clear();
}

void Level::checkCollisions() {

  // Collisions of objects with map
  for (auto obj : objects_) {
    if (obj->getType() == GameObjectType::Fire) {
      glm::vec2 collision_point;
      if (map_->isCollision(obj->getCollisionArea(), &collision_point)) {
        obj->onCollision(nullptr);

        // Debug
        PointWithTimer point;
        point.point = collision_point;
        point.expiration = Clock::now() + 2s;
        collision_points_.push_back(point);
      }
    }
  }

  // Objects with objects
  size_t objects_size = objects_.size();
  for (int i = 0; i < (objects_size - 1); i++) {
    GameObject* obj1 = (&objects_[i])->get();
    for (int j = i + 1; j < objects_size; j++) {
      GameObject* obj2 = (&objects_[j])->get();
      glm::vec2 collision_point;
      if (Collision::isCollision(obj1->getCollisionArea(), obj2->getCollisionArea(), &collision_point)) {
        // Debug
        PointWithTimer point;
        point.point = collision_point;
        point.expiration = Clock::now() + 2s;
        collision_points_.push_back(point);
        
        LOG_INFO("Collision!! " << rand());
        obj1->onCollision(obj2);
        obj2->onCollision(obj1);
      }
    }
  }
}

void Level::setOpenGlLights() {
  glm::vec3 light_pos{0.0f, 0.3f, 1.f};
  light_pos = glm::normalize(light_pos);
  float light_position[4];
  light_position[0] = light_pos.x;
  light_position[1] = light_pos.y;
  light_position[2] = light_pos.z;
  light_position[3] = 0.f;  // w=0-> vector


  float light_diffuse[] = {0.2f, 0.2f, 0.2f, 1.f};
  float light_ambient[] = {0.1f, 0.1f, 0.1f, 1.f};
  float light_specular[] = {0.f, 0.f, 0.f, 1.f};
  diffuse_coeff_ = 0.3f;
  ambient_coeff_ = 0.5f;
  for (int i = 0; i < 3; i++) {
    light_diffuse[i] = diffuse_coeff_;
    light_ambient[i] = ambient_coeff_;
  }

  GLfloat lmodel_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void Level::render() {
  int width = 0;
  int height = 0;
  ResourcesManager::getInstance().getWindowDimensions(&width, &height);
  glShadeModel(GL_FLAT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setOpenGlLights();
 
  glEnable(GL_NORMALIZE);
  
  GLfloat ratio = (GLfloat)width / (GLfloat)height;

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, ratio, 0.5f, 100.0f);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  camera_.apply();
  
  OpenGlResources::drawAxis();
  glColor3f(1.f, 0.f, 0.f);
  for (auto &point : collision_points_) {
    glPushMatrix();
    glTranslatef(point.point.x, point.point.y, 0);
    OpenGlResources::drawCircle(1, 10);
    glPopMatrix();
  }
  collision_points_.erase(std::remove_if(collision_points_.begin(), collision_points_.end(),
    [](const PointWithTimer& o) {return o.expiration < Clock::now(); }), collision_points_.end());

  
  if (render_objects_) map_->render();
  if (render_collision_area_) map_->renderCollisionArea();

  // Draw object
  for (auto obj : objects_) {
    glPushMatrix();
    if (render_collision_area_) obj->renderCollisionArea();
    if (render_objects_) obj->render();
    glPopMatrix();
  }
}
