#include "SDL.h"

#include "GameObject.h"

class Fire : public GameObject {
public:
  Fire(const GameObject& origin) : GameObject(origin, GameObjectType::Fire) { 
  };
  void update(const Uint8* keys, uint32_t elapsed_us) override;
  void render() override;
  void onCollision(GameObject* with) override;


private:
  // m/s
  float vel_x_ = 15.0;
  float vel_y_ = 15.0;

};