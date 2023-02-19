#include <glad/glad.h>
#include <util.hpp>
#include <mesh.hpp>

gl::Mesh::Mesh()
{
  vbo = 0;
  vao = 0;
}

gl::Mesh::Mesh
( const Float32 * vertexBuffer,
  const UInt32    pvertices,
  const UInt32  * attributes,
  const UInt32    pprimitive) : vertices(pvertices), primitive(pprimitive)
{
  vbo = 0;
  vao = 0;

  SInt32 vertexSize = 0;
  for(SInt32 i = 0; attributes[i]; i++)
      vertexSize += attributes[i];

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Float32)*vertexSize*vertices,
               vertexBuffer,
               GL_STATIC_DRAW);

  UInt32 offset = 0;
  for (UInt32 i = 0; attributes[i]; i++)
  {
    UInt32 size = attributes[i];
    glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE,
                          vertexSize*sizeof(Float32), (const void*)(offset*sizeof(Float32)));
    glEnableVertexAttribArray(i);
    offset += size;
  }

  glBindVertexArray(0);
}

gl::Mesh::~Mesh()
{
  if(vao != 0) glDeleteVertexArrays(1, &vao);
  if(vbo != 0) glDeleteBuffers(1, &vbo);
}

void gl::Mesh::draw()
{
  glBindVertexArray(vao);
  glDrawArrays(primitive, 0, vertices);
  glBindVertexArray(0);
}