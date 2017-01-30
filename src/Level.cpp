#include "Level.h"

#include <thread>

#include <GL/glew.h>

#include "ResourcesManager.h"
#include "Logger.h"
#include "Slider.h"
#include "Fire.h"
#include "Map.h"

Level::Level(Map* map) : map_(map) {
  // Add objects to the level
  objects_.push_back(std::make_shared<SliderLocalPlayer>(map->getPlayerInitialPosition(), 0.0f));
  auto enemy_positions = map->getEnemiesInitialPositions();
  for (auto &enemy_position : enemy_positions) {
    objects_.push_back(std::make_shared<SliderComputerEnemy>(enemy_position, 180.0f));
  }

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

void Level::update(const Uint8* keys, uint32_t elapsed_us) {
  // Pause!!!
  if (keys[SDL_SCANCODE_P]) {
    return;
  }
  loop_count_++;

  addPendingObjects();

  for (auto obj : objects_) {
    obj->update(keys, elapsed_us);
  }

  // Check boundaries with map
  for (auto obj : objects_) {
    if (obj->getType() == GameObjectType::Fire) {
      if (map_->isCollision(obj.get())) {
        obj->onCollision(nullptr);
      }
    }
  }

  removePendingObjects();

  checkCollisions();
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
  size_t objects_size = objects_.size();
  for (int i = 0; i < (objects_size - 1); i++) {
    GameObject* obj1 = (&objects_[i])->get();
    for (int j = i + 1; j < objects_size; j++) {
      GameObject* obj2 = (&objects_[j])->get();
      if (GameObject::isCollision(obj1, obj2)) {
        LOG_INFO("Collision!! " << rand());
        obj1->onCollision(obj2);
        obj2->onCollision(obj1);
      }
    }
  }
}

void Level::render() {
  int width = 0;
  int height = 0;
  ResourcesManager::getInstance().getWindowDimensions(&width, &height);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  GLfloat ratio;

  ratio = (GLfloat)width / (GLfloat)height;

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, ratio, 0.1f, 400.0f);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  // Camera
 // gluLookAt(1, 0, 5, 1, 1, 0, 0, 0, 1);
  gluLookAt(-5, -5, 20, 20, 20, 0, 0, 0, 1);

  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
  glColor3f(0, 1, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0, 0, 1);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
  glEnd();

  map_->render();

  // Draw object
  for (auto obj : objects_) {
    glPushMatrix();
    obj->render();
    glPopMatrix();
  }
}
