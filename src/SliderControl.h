#pragma once

#include <cstdint>

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
};

class SliderControl {
public: 
  virtual void update(uint32_t elapsed_us) = 0;
  virtual SliderCommands getCommands() = 0;
};

class ManualSliderControl : public SliderControl {
  void update(uint32_t elapsed_us) override {};
  SliderCommands getCommands() override;
};

class AiSliderControl : public SliderControl {
  void update(uint32_t elapsed_us) override {};
  SliderCommands getCommands() override;
};