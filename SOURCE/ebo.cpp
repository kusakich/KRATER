#include <glad/glad.h>
#include <util.hpp>
#include <ebo.hpp>

gl::Ebo::Ebo()
{
  glGenBuffers(1, &id);
}

gl::Ebo::~Ebo()
{
  glDeleteBuffers(1, &id);
}

void gl::Ebo::bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void gl::Ebo::unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void gl::Ebo::data(const Float32 * data, const UInt32 size)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}