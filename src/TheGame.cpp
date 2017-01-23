#include "TheGame.h"

#include <thread>
#include <vector>
#include <cassert>

#include "OpenGlResources.h"
#include "ResourcesManager.h"
#include "Logger.h"
#include "Time.h"

TheGame::TheGame() {
  window_ = std::make_unique<OpenGlWindow>();
}

TheGame::~TheGame() {
  window_.reset(nullptr);
  SDL_JoystickClose(joystick_);
  SDL_Quit();
}

bool TheGame::init() {
  /*
  if (!SDL_InitSubSystem())
  int num_joysticks = SDL_NumJoysticks();
  if (num_joysticks == 0) {
    LOG_INFO("No joysticks found");
  } else {
    joystick_ = SDL_JoystickOpen(0);
    if (joystick_) {
      LOG_INFO("Joystick found: " << SDL_JoystickName(joystick_));
    } else {
      LOG_ERROR("Could not open joystick");
    }
  }
  */
  int width = 640;
  int height = 480;

  if (!window_->create(width, height)) {
    return false;
  }

  // --------------------------------
  // OpenGL context is available now!
  // --------------------------------

  // Load resources
  if (!ResourcesManager::getInstance().loadModels()) {
    LOG_ERROR("Could not load models");
    return false;
  }

  if (!ResourcesManager::getInstance().loadMaps()) {
    LOG_ERROR("Could not load maps");
    return false;
  }

  stage_ = std::make_unique<Level>(ResourcesManager::getInstance().getMap());

  return true;
}

void TheGame::processEvents(std::vector<SDL_Event>& events, bool *done) {
  for (auto& e : events) {
    // Process events
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
      *done = true;
    }

    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
 //     LOG_DEBUG("Key : " << e.key.keysym.sym << " " << (e.type == SDL_KEYUP ? "UP" : "DOWN"));
    }

    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
      LOG_DEBUG("Window resized: " << e.window.data1 << "x" << e.window.data2);
      ResourcesManager::getInstance().setWindowDimensions(e.window.data1, e.window.data2);
    }
    if (e.type == SDL_JOYBUTTONDOWN) {
      LOG_DEBUG("Joy button");
    }
   /* if (e.type == SDL_JOYAXISMOTION) {
      LOG_DEBUG("Joy axis");
    }*/
  }
}

void TheGame::runLoop() {
  if (!stage_) {
    LOG_ERROR("No stage loaded");
    return;
  }
  bool done = false;
  SDL_Event event;
  std::vector<SDL_Event> events;
  int counter = 0;
  Duration time_per_cycle = std::chrono::milliseconds{1000 / target_fps_};
  Duration lag = std::chrono::milliseconds{0};
  TimePoint last_update_call;

  while (!done) {

    TimePoint start = Clock::now();

    counter++;
    events.clear();
    while (SDL_PollEvent(&event)) {
      events.push_back(event);  // Get all the events now and process later
    }
    processEvents(events, &done);
    TimePoint current_time = Clock::now();
    Duration elapsed_between_update = counter == 1 ? time_per_cycle : current_time - last_update_call;
    stage_->update(
      SDL_GetKeyboardState(nullptr), 
      (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(elapsed_between_update).count());
    last_update_call = current_time;
    stage_->render();  // TODO: we are assuming render time is short. Add condition to discard render if time is critical
    window_->display();
    Duration elapsed = Clock::now() - start;

    Duration time_left = time_per_cycle - elapsed;
    if (time_left > 0s) {
      std::this_thread::sleep_for(time_left);
    } else {
      LOG_ERROR("Time for cycle exceeded max: " << counter << " " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
    }
  }
}
