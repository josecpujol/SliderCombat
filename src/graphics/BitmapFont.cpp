#include "BitmapFont.h"

#include "SDL_image.h"
#include "tinyxml2.h"

BitmapFont::~BitmapFont() {
  for (auto page : pages_) {
    glDeleteTextures(1, &page.second);
  }
}

void BitmapFont::render(const std::string& text, int x, int y, int size) {
  float x_texture, y_texture, w_texture, h_texture;
  float x_rect, y_rect, w_rect, h_rect;

  float scale_factor = (float)size / line_height_;
  
  float x_cursor = x;
  float y_cursor = y;


  glColor3f(1.f, 1.f, 1.f);  // The color is affected by glColor, unless glTexEnv is set with magic params
  glBegin(GL_QUADS);

  for (auto character : text) {
    CharInfo* char_info = &(chars_[character]);
    glBindTexture(GL_TEXTURE_2D, char_info->texture);
    x_texture = ((float)char_info->x) / textures_width_;
    y_texture = ((float)char_info->y) / textures_height_;
    w_texture = ((float)char_info->width) / textures_width_;
    h_texture = ((float)char_info->height) / textures_height_;

    x_rect = x_cursor + scale_factor * char_info->xoffset;
    y_rect = y_cursor + scale_factor * char_info->yoffset;
    w_rect = scale_factor * char_info->width;
    h_rect = scale_factor * char_info->height;

    glTexCoord2f(x_texture, y_texture);               glVertex2f(x_rect, y_rect );
    glTexCoord2f(x_texture + w_texture, y_texture);   glVertex2f(x_rect + w_rect, y_rect);
    glTexCoord2f(x_texture + w_texture, y_texture + h_texture); glVertex2f(x_rect + w_rect, y_rect + h_rect);
    glTexCoord2f(x_texture, y_texture + h_texture); glVertex2f(x_rect, y_rect + h_rect);
    
    x_cursor += (scale_factor * char_info->xadvance);
  }
  glEnd();
}

bool BitmapFont::load(const std::string& xml_description) {
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(xml_description.c_str()) != tinyxml2::XML_SUCCESS) {
    return false;
  }
  tinyxml2::XMLElement* font_element = doc.FirstChildElement("font");
  if (!font_element) {
    return false;
  }

  if (!loadHeader(font_element)) {
    return false;
  }
  
  std::map<std::string, std::string> pages = loadPages(font_element);
  if (pages.empty()) {
    return false;
  }

  // Load pngs
  std::size_t found = xml_description.find_last_of("/");
  std::string parent_path = xml_description.substr(0, found) + "/";
  for (auto page : pages) {
    std::string png_file = parent_path + std::string(page.second);
    SDL_Surface* surface = IMG_Load(png_file.c_str());
    if (!surface) return false;

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    pages_[page.first] = texture_id;

    SDL_FreeSurface(surface);
  }

  loadChars(font_element);

  return true;
}

std::map<std::string, std::string> BitmapFont::loadPages(tinyxml2::XMLElement* font_element) {
  std::map<std::string, std::string> pages;

  tinyxml2::XMLElement* pages_element = font_element->FirstChildElement("pages");
  if (!pages_element) {
    return{};
  }
  tinyxml2::XMLElement* page_element = pages_element->FirstChildElement("page");
  while (page_element != nullptr) {
    pages[page_element->Attribute("id")] = page_element->Attribute("file");
    page_element = page_element->NextSiblingElement("page");
  }
  return std::move(pages);
}

bool BitmapFont::loadChars(tinyxml2::XMLElement* font_element) {
  std::map<std::string, std::string> pages;

  tinyxml2::XMLElement* chars_element = font_element->FirstChildElement("chars");
  if (!chars_element) {
    return false;
  }
  tinyxml2::XMLElement* char_element = chars_element->FirstChildElement("char");
  while (char_element != nullptr) {
    CharInfo info;
    info.x = std::atoi(char_element->Attribute("x"));
    info.y = std::atoi(char_element->Attribute("y"));
    info.xadvance = std::atoi(char_element->Attribute("xadvance"));
    info.xoffset = std::atoi(char_element->Attribute("xoffset"));
    info.yoffset = std::atoi(char_element->Attribute("yoffset"));
    info.height = std::atoi(char_element->Attribute("height"));
    info.width = std::atoi(char_element->Attribute("width"));
    info.texture = pages_[char_element->Attribute("page")];
    
    chars_[(char)std::stoi(char_element->Attribute("id"))] = info;
    
    char_element = char_element->NextSiblingElement("char");
  }
  return true;
}

bool BitmapFont::loadHeader(tinyxml2::XMLElement* font_element) {
  tinyxml2::XMLElement* common = font_element->FirstChildElement("common");
  textures_height_ = std::atoi(common->Attribute("scaleH"));
  textures_width_ = std::atoi(common->Attribute("scaleW"));
  line_height_ = std::atoi(common->Attribute("lineHeight"));
  return true;
}