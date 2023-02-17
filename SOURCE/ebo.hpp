#ifndef EBO_HPP_INCLUDED
#define EBO_HPP_INCLUDED

#include <util.hpp>

namespace gl
{
  class Ebo
  {
  private:
    UInt32 id;
  public:
    Ebo();
    ~Ebo();
    void bind();
    void unbind();
    void data(const Float32 * data,
              const UInt32    size);
  };
}

#endif