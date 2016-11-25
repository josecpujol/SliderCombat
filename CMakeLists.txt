cmake_minimum_required (VERSION 3.0)
project (SliderCombar)

set (CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
SET(GLM_INCLUDE_DIRS "/usr/include/glm")


find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("GLEW FOUND")
    include_directories(${GLEW_INCLUDE_DIRS})
    message(${GLEW_INCLUDE_DIRS})
endif()

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})


FILE(GLOB MyCppSources src/*.cpp)
ADD_EXECUTABLE(SliderCombat ${MyCppSources})
MESSAGE(${SDL2_LIBRARIES})
TARGET_LINK_LIBRARIES(SliderCombat "GL" "GLU" ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})
