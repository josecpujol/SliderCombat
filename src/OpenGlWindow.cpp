#include "OpenGlWindow.h"

#include "Logger.h"
#include "ResourcesManager.h"

OpenGlWindow::~OpenGlWindow() {
  LOG_DEBUG("Destroying OpenGlWindow");
  SDL_GL_DeleteContext(gl_context_);
  SDL_DestroyWindow(window_);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool OpenGlWindow::create(int width, int height) {

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    LOG_ERROR("Could not init video");
    return false;
  }

  //Use OpenGL 2.1 core
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#ifndef __EMSCRIPTEN__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  window_ = SDL_CreateWindow("Slider Combat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window_) {
    LOG_ERROR("Unable to create window: " << SDL_GetError());
    return false;
  }

  ResourcesManager::getInstance().setWindowDimensions(width, height);

  gl_context_ = SDL_GL_CreateContext(window_);
  if (gl_context_ == nullptr) {
    LOG_ERROR("Unable to create gl context. Message: " << SDL_GetError());
    return false;
  }
  LOG_DEBUG("Window and opengl context created successfully");

  if (SDL_GL_SetSwapInterval(1) != 0) {
    LOG_ERROR("Could not set swap interval: " << SDL_GetError());
  //  return false;
  }

  // Init glew and stuff
  if (!opengl_.init()) {
    LOG_ERROR("Unable to initialize opengl resources");
    return false;
  }

  return true;
}

void OpenGlWindow::display() {
  SDL_GL_SwapWindow(window_);
}
