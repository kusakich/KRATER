#include <glad/glad.h>
#include <util.hpp>
#include <mesh.hpp>

gl::Mesh::Mesh()
{
  vbo = nullptr;
  ebo = nullptr;
  vao = nullptr;
}

gl::Mesh::Mesh
( const Float32 * vertexBuffer,
  const UInt32    pvertices,
  const UInt32  * attributes,
  const UInt32    pprimitive) : vertices(pvertices), primitive(pprimitive)
{
  vbo = nullptr;
  ebo = nullptr;
  vao = nullptr;

  SInt32 vertexSize = 0;
  for(SInt32 i = 0; attributes[i]; i++)
      vertexSize += attributes[i];

  vao = new Vao();
  vao->bind();

  vbo = new Vbo();
  vbo->data(vertexBuffer, sizeof(Float32)*vertexSize*vertices);
  vbo->bind();

  UInt32 offset = 0;
  for (UInt32 i = 0; attributes[i]; i++)
  {
    UInt32 size = attributes[i];
    vao->attrib(i, size, vertexSize*sizeof(Float32), (const void*)(offset*sizeof(Float32)));
    vao->enable(i);
    offset += size;
  }
  vbo->unbind();
  vao->unbind();
}

gl::Mesh::~Mesh()
{
  if(vao != nullptr) delete vao;
  if(vbo != nullptr) delete vbo;
  if(ebo != nullptr) delete ebo;
}

void gl::Mesh::draw()
{
  if(vao != nullptr && vbo != nullptr)
  {
    vao->bind();
    glDrawArrays(primitive, 0, vertices);
    vao->unbind();
  }
}

void gl::Mesh::setVbo(Vbo * pvbo)
{
  if(vbo != nullptr)
    delete vbo;
  vbo = pvbo;
}

void gl::Mesh::setEbo(Ebo * pebo)
{
  if(ebo != nullptr)
    delete ebo;
  ebo = pebo;
}

void gl::Mesh::setVao(Vao * pvao)
{
  if(vao != nullptr)
    delete vao;
  vao = pvao;
}