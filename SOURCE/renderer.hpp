#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <util.hpp>
#include <primitives.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <camera.hpp>

namespace gl
{
  namespace renderer
  {
    void initialize();
    void finalize();
    void drawRect(Rect rect);
    void drawQuad(Quad quad, Camera camera);
    void drawText(std::string text, Float32 x, Float32 y, Float32 scale);

    template<typename T> Mesh* renderObject(T object);
  };
}

#endif