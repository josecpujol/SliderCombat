#pragma once

class Object3d;
class OpenGlProgram;
class Scene;

class OpenGlRenderer {
public:
  void render(Object3d*, OpenGlProgram*, Scene*);
};