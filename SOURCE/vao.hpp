#ifndef VAO_HPP_INCLUDED
#define VAO_HPP_INCLUDED

#include <util.hpp>

namespace gl
{
  class Vao
  {
  private:
    UInt32 id;
  public:
    Vao();
    ~Vao();
    void bind();
    void unbind();
    void enable(UInt32 index);
    void disable(UInt32 index);
    void attrib(UInt32 index,   UInt32      size,
                UInt32 stride,  const void* data);
  };
}

#endif