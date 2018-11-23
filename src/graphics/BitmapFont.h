#pragma once

#include <string>
#include <map>
#include "OpenGlResources.h"

namespace tinyxml2 {
  class XMLElement;
};

struct CharInfo {
  int x;
  int y;
  int width;
  int height;
  int xoffset;
  int yoffset;
  int xadvance;
  GLuint texture;
};

class BitmapFont {
public:
  BitmapFont() {}
  ~BitmapFont();
  bool load(const std::string& xml_description);
  void render(const std::string& text, int x, int y, int size);  // x, y: in pixels. Origin: top left

private:
  std::map<std::string, GLuint> pages_;
  std::map<std::string, std::string> loadPages(tinyxml2::XMLElement* font_element);
  bool loadChars(tinyxml2::XMLElement* font_element);
  bool loadHeader(tinyxml2::XMLElement* font_element);
  std::map<char, CharInfo> chars_;
  int textures_width_;
  int textures_height_;
  int line_height_;
};