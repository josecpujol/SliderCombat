#include "Level.h"

#include <thread>

#include <GL/glew.h>
#include <glm/gtx/norm.hpp>

#include "ResourcesManager.h"
#include "Logger.h"
#include "Slider.h"
#include "Fire.h"

Level::Level() {
  // Add objects to the level
  objects_.push_back(std::make_shared<SliderLocalPlayer>(glm::vec2(10.0, 0.0), 0.0f));
  objects_.push_back(std::make_shared<SliderComputerEnemy>(glm::vec2(0.0, 0.0), 180.0f));

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
  loop_count_++;

  addObjects();

  for (auto obj : objects_) {
    obj->update(keys, elapsed_us);
  }

  // Check boundaries with map
  for (auto obj : objects_) {
    const glm::vec3& pos3 = obj->getPosition();
    glm::vec2 pos2 = glm::abs(glm::vec2(pos3));
    if (pos2.x > 50 || pos2.y > 50) {
      if (obj->getType() == GameObjectType::Fire) {
        obj->onCollision(nullptr);
      }
    }
  }

  removeObjects();
  
  checkCollisions();
}

void Level::addObjects() {
  for (auto obj : objects_to_add_) {
    objects_.push_back(obj);
  }
  objects_to_add_.clear();
}

void Level::removeObjects() {
  for (auto object_to_remove : objects_to_remove_) {
    auto i = std::begin(objects_);

    auto size_objects_before = objects_.size();
    while (i != std::end(objects_)) {
      if (i->get() == object_to_remove) {
        i = objects_.erase(i);
      }
      else {
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
    for (int j = i + 1; j < objects_size; j++) {
      checkCollision((&objects_[i])->get(), (&objects_[j])->get());
    }
  }
}

// TODO: improve collision check
void Level::checkCollision(GameObject* obj1, GameObject* obj2) {
  float distance2 = glm::distance2(obj1->getPosition(), obj2->getPosition());
  float radius_sum = obj1->getRadius() + obj2->getRadius();
  if (distance2 < radius_sum * radius_sum) {
    LOG_INFO("Collision!! "  << rand());
    obj1->onCollision(obj2);
    obj2->onCollision(obj1);
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

  gluPerspective(45.0f, ratio, 0.1f, 200.0f);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  // Camera
  gluLookAt(5, -20, 20, 0, 0, 0, 0, 0, 1);

  // draw floor
  glColor3f(1, 1, 1);
  glPushMatrix();
  glTranslatef(-50., -50., 0);  // center
  glBegin(GL_LINES);
  for (int i = 0; i < 100; i++) {
    glVertex3f((float)i, 0, 0.0f); glVertex3f((float)i, 100, 0);
    glVertex3f(0, (float)i, 0.0f); glVertex3f(100, (float)i, 0);
  }
  glEnd();
  glPopMatrix();
  
  // Draw object
  for (auto obj : objects_) {
    glPushMatrix();
    obj->render();
    glPopMatrix();
  }
}
