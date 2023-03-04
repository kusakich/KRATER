#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <util.hpp>
#include <primitives.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <camera.hpp>

namespace renderer
{
  void initialize(Window* pwindow, std::string fontFile);
  void finalize();
  void drawRect(Rect rect);
  void drawQuad(Quad quad, Camera* camera);
  void drawText(std::string text, Float32 x, Float32 y, Float32 scale);

  gl::Mesh* renderChunk(World* world, glm::uvec2 chunkIndex);

  glm::vec3 raycast(Camera* camera, Float32 delta);
};

#endif