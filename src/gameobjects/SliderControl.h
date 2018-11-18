#pragma once

#include <cstdint>
#include <sstream>

#include "utils/Time.h"
#include "mathnphysics/Math.h"

class Slider;

struct SliderCommands {
  bool left_thruster_foward = false;
  bool left_thruster_backwards = false;
  bool left_thruster_left = false;
  bool left_thruster_right = false;

  bool right_thruster_foward = false;
  bool right_thruster_backwards = false;
  bool right_thruster_left = false;
  bool right_thruster_right = false;

  bool fire = false;

  SliderCommands& operator+=(const SliderCommands& rhs);
};

class SliderControl {
public: 
  virtual SliderCommands update(uint32_t elapsed_us) = 0;
};

class ManualSliderControl : public SliderControl {
  SliderCommands update(uint32_t elapsed_us) override;
};

class AiSliderControl : public SliderControl {
public:
  AiSliderControl(Slider* slider) : slider_(slider) {};
  SliderCommands update(uint32_t elapsed_us) override;
private:
  SliderCommands rotateTo(const glm::vec2& to);
  SliderCommands moveTo(const glm::vec2& to);
  Duration elapsed_time_ = 0s;
  Slider* slider_;
};