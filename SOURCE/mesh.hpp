#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <util.hpp>
#include <vbo.hpp>
#include <vao.hpp>
#include <ebo.hpp>

namespace gl
{
  class Mesh
  {
  private:
    Vbo * vbo;
    Ebo * ebo;
    Vao * vao;

    UInt32 vertices;
    UInt32 primitive;
  public:
    Mesh();
    Mesh( const Float32 * vertexBuffer,
          const UInt32    pvertices,
          const UInt32  * attributes,
          const UInt32    pprimitive);
    ~Mesh();
    void draw();

    void setVbo(Vbo * pvbo);
    void setEbo(Ebo * pebo);
    void setVao(Vao * pvao);
  };
}

#endif