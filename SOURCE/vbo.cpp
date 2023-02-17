#include <glad/glad.h>
#include <util.hpp>
#include <vbo.hpp>

gl::Vbo::Vbo()
{
  glGenBuffers(1, &id);
}

gl::Vbo::~Vbo()
{
  glDeleteBuffers(1, &id);
}

void gl::Vbo::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

void gl::Vbo::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl::Vbo::data(const Float32 * data, const UInt32 size)
{
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}