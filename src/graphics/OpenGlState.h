/**
 * Singleton to keep global "opengl state": modelview matrix, lights, program in use, etc
 */

class OpenGlState {
public:
  OpenGlState(OpenGlState const&) = delete;
  void operator=(OpenGlState const&) = delete;

  static OpenGlState& getInstance() {
    static OpenGlState instance;
    return instance;
  }

private:
  OpenGlState() = default;
}