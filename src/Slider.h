#include "SDL.h"

#include <vector>

#include "GameObject.h"
#include "Time.h"

class Slider : public GameObject {
public:
  Slider() = delete;
  Slider(GameObjectType type, const glm::vec2& pos, float rot) : GameObject(GameObjectType::LocalPlayer, glm::vec3(pos, 0.1), rot) {
  };
  void update(const Uint8* keys, uint32_t elapsed_us) override {};
  void render() override;
  void onCollision(GameObject* with) override;


protected:
  // m/s
  float vel_x_ = 5.0;
  float vel_y_ = 5.0;
  float vel_rot_ = 20.0;
  Duration shot_cadence_ = 200ms;
  TimePoint last_shot_;
};

class SliderLocalPlayer : public Slider {
public:
  SliderLocalPlayer() = delete;
  SliderLocalPlayer(const glm::vec2& pos, float rot) : Slider(GameObjectType::LocalPlayer, pos, rot) {
  };

  void update(const Uint8* keys, uint32_t elapsed_us) override;
};

class SliderComputerEnemy : public Slider {
public:
  SliderComputerEnemy() = delete;
  SliderComputerEnemy(const glm::vec2& pos, float rot) : Slider(GameObjectType::ComputerEnemy, pos, rot) {
    keys_state_.resize(7);
  };
  void update(const Uint8* keys, uint32_t elapsed_us) override;

private:
  std::vector<bool> keys_state_;
};