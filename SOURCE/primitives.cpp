#include <util.hpp>
#include <primitives.hpp>

Rect::Rect(UInt32 x, UInt32 y, UInt32 width, UInt32 height) : position(x,y), size(width, height) {}
Rect::Rect(glm::ivec2 pposition, glm::ivec2 psize) : position(pposition), size(psize) {}
Quad::Quad(glm::vec3 pposition, glm::vec3 psize) : position(pposition), size(psize) {}