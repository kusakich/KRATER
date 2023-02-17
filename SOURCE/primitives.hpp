#ifndef CUBE_HPP_INCLUDED
#define CUBE_HPP_INCLUDED

#include <util.hpp>

struct Rect
{
  Rect(UInt32 x = 0, UInt32 y = 0, UInt32 width = 100, UInt32 height = 100);
  Rect(glm::ivec2 pposition = glm::ivec2(0), glm::ivec2 psize = glm::ivec2(100));
  glm::ivec2 position;
  glm::ivec2 size;
};

struct Quad
{
  Quad(glm::vec3 pposition = glm::vec3(0.0), glm::vec3 psize = glm::vec3(1.0));
  glm::vec3 position;
  glm::vec3 size;
};

#endif