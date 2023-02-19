#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <util.hpp>

namespace gl
{
  class Mesh
  {
  private:
    UInt32 vbo, vao;
    UInt32 vertices;
    UInt32 primitive;
  public:
    Mesh();
    Mesh(const Float32 * vertexBuffer,
         const UInt32    pvertices,
         const UInt32  * attributes,
         const UInt32    pprimitive);
    ~Mesh();
    void draw();
  };
}

#endif