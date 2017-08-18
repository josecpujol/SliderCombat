#include "TheGame.h"

#include <thread>
#include <vector>
#include <cassert>

#include "OpenGlResources.h"
#include "ResourcesManager.h"
#include "Logger.h"
#include "Time.h"
#include "Stats.h"



TheGame::TheGame() {
  window_ = std::make_unique<OpenGlWindow>();
}

TheGame::~TheGame() {
  // Release all resources: all the managers have some opengl stuff, so release them before deleting the context (window)
  // call all singletons to release opengl resources
  ResourcesManager::getInstance().releaseResources();
  LoggerOpenGl::getInstance().releaseResources();

  window_.reset(nullptr);
  TTF_Quit();
  SDL_JoystickClose(joystick_);
  SDL_Quit();
}

bool TheGame::init() {
  
  if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
    LOG_ERROR("Could not initialize gamecontroller");
    return false;
  }
  int num_joysticks = SDL_NumJoysticks();
  LOG_INFO("#joysticks found: " << num_joysticks);
  for (int i = 0; i < num_joysticks; i++) {
    if (SDL_IsGameController(i)) {
      LOG_DEBUG("Joystick " << i << " is supported by the game controller interface");
    } else {
      LOG_DEBUG("Joystick " << i << " is NOT supported by the game controller interface");
    }
    LOG_DEBUG("Joystick " << i << ": " << SDL_JoystickNameForIndex(i));
    joystick_ = SDL_JoystickOpen(i);
    if (joystick_) {
      LOG_INFO("Joystick found: " << SDL_JoystickName(joystick_));
      break;
    } else {
      LOG_ERROR("Could not open joystick. Error: "  << SDL_GetError());
    }
  }

  // Text
  if (TTF_Init() < 0) {
    LOG_ERROR("Could not initialize text");
    return false;
  }
  
  int width = 800;
  int height = 600;

  if (!window_->create(width, height)) {
    return false;
  }

  // --------------------------------
  // OpenGL context is available now!
  // --------------------------------

  if (!ResourcesManager::getInstance().loadResources()) {
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
 /*   if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_z) stage_->incAmbient(0.1f);
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_x) stage_->incAmbient(-0.1f);
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_c) stage_->incDiffuse(0.1f);
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_v) stage_->incDiffuse(-0.1f);
  */

    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
      LOG_DEBUG("Window resized: " << e.window.data1 << "x" << e.window.data2);
      ResourcesManager::getInstance().setWindowDimensions(e.window.data1, e.window.data2);
    }
    if (e.type == SDL_JOYBUTTONDOWN) {
      LOG_DEBUG("Joy button");
    }
    if (e.type == SDL_JOYAXISMOTION) {
      LOG_DEBUG("Joy axis");
    }
  }
}

void TheGame::runLoop() {
  if (!stage_) {
    LOG_ERROR("No stage loaded");
    return;
  }
  bool done = false;
  SDL_Event event;
  int counter = 0;
  Duration time_per_cycle = std::chrono::milliseconds{1000 / target_fps_};
  Duration lag = std::chrono::milliseconds{0};
  TimePoint last_update_call;

  Stats& stats = Stats::getInstance();
  stats.reset();

  while (!done) {
    counter++;

    TimePoint start = Clock::now();

    Duration poll_and_process_events_duration = measureFunction([&done, &event, this]{
     std::vector<SDL_Event> events;
     TimePoint start = Clock::now();
     while (SDL_PollEvent(&event) && (Clock::now() - start) < 2ms) {
       events.push_back(event);  // Get all the events now and process later
     }
     processEvents(events, &done);
    });

    Duration update_duration = measureFunction([this, &time_per_cycle] {
      stage_->update(
        SDL_GetKeyboardState(nullptr),
        (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(time_per_cycle).count());
    });
    stats.time_to_update_ms += toMs(update_duration);

    Duration render_duration = measureFunction([this] {
      stage_->render();
      LoggerOpenGl::getInstance().render();
      glFinish();
    });

    Duration display_duration = measureFunction([this] {
      window_->display();

    });
    stats.time_to_render_ms += toMs(render_duration);
    stats.frames_rendered++;

    Duration elapsed = (Clock::now() - start);
    Duration time_left = time_per_cycle - elapsed;
    /*if (time_left > 0s) {
    } else { 
      LOG_ERROR_SCREEN("Time for cycle exceeded max. Frame #" << counter
        << ", time taken: " << toMs(elapsed)
        << ", time per frame: " << toMs(time_per_cycle)
        << ", poll_and_process_events: " << toMs(poll_and_process_events_duration)
        << ", update: " << toMs(update_duration)
        << ", render: " << toMs(render_duration)
        << ", display: " << toMs(display_duration));
   // } */
    int counter_max = 100;
    if (counter % counter_max == 0) {
      LOG_SCREEN_N(0, "Fps: " << (1000 * stats.frames_rendered) / toMs(Clock::now() - stats.reset_time));
      LOG_SCREEN_N(1, "Avg triangles rendered: " << stats.num_triangles / counter_max);
      LOG_SCREEN_N(2, "Avg render time ms: " << stats.time_to_render_ms / counter_max);
      LOG_SCREEN_N(3, "Avg update time ms: " << stats.time_to_update_ms / counter_max);
      stats.reset();
    }
  }
}
