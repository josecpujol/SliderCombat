class ResourcesManager {
public:
  ResourcesManager(ResourcesManager const&) = delete;
  void operator=(ResourcesManager const&) = delete;
  
  static ResourcesManager& getInstance() {
    static ResourcesManager instance;
    return instance;
  }

  void setWindowDimensions(int w, int h);
  void getWindowDimensions(int* w, int* h);
private:
  ResourcesManager() {}

  int window_width_ = 0;
  int window_height_ = 0;
};