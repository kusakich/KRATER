#ifndef VBO_HPP_INCLUDED
#define VBO_HPP_INCLUDED

#include <util.hpp>

namespace gl
{
  class Vbo
  {
  private:
    UInt32 id;
  public:
    Vbo();
    ~Vbo();
    void bind();
    void unbind();
    void data(const Float32 * data,
              const UInt32    size);
  };
}

#endif