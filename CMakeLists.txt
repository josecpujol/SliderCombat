cmake_minimum_required (VERSION 3.0)
project (SliderCombar)

set (CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS} -DUSE_MINIZ")

MESSAGE("CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
  message("Emscripten detected")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s LEGACY_GL_EMULATION=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --embed-file ../data")
else () 
  INCLUDE(FindPkgConfig)
  PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
  PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
  PKG_SEARCH_MODULE(GLEW REQUIRED glew)
endif ()

SET(GLM_INCLUDE_DIRS "/usr/include/glm")

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} "src" "ext/tinyobjloader" "ext/tinyxml" "ext/tmxparser" )

FILE(GLOB_RECURSE MySources ext/*.cpp ext/*.c src/*.cpp test/*.cpp)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

ADD_EXECUTABLE(SliderCombat ${MySources})
if (NOT EMSCRIPTEN)
  TARGET_LINK_LIBRARIES(SliderCombat ${GLEW_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
endif(NOT EMSCRIPTEN)
