#include <glad/glad.h>
#include <util.hpp>
#include <vao.hpp>

gl::Vao::Vao()
{
  glGenVertexArrays(1, &id);
}

gl::Vao::~Vao()
{
  glDeleteVertexArrays(1, &id);
}

void gl::Vao::bind()
{
  glBindVertexArray(id);
}

void gl::Vao::unbind()
{
  glBindVertexArray(0);
}

void gl::Vao::enable(UInt32 index)
{
  glEnableVertexAttribArray(index);
}

void gl::Vao::disable(UInt32 index)
{
  glDisableVertexAttribArray(index);
}

void gl::Vao::attrib(UInt32 index, UInt32 size, UInt32 stride, const void* data)
{
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, data);
}