#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <util.hpp>

namespace gl
{
  class Shader
  {
  public:
    enum Type
    {
      VERTEX,
      FRAGMENT,
      GEOMETRY,
      TESSELATION,
      COMPUTE
    };
  private:
    UInt32  id_;
    Type    type_;
  public:
    UInt32  const & id;
    Type    const & type;
    Shader(const char* fileName, Type ptype);
    ~Shader();
  };

  class ShaderProgram
  {
  private:
    UInt32 id_;
    std::map<std::string, SInt32> uniforms_;
  public:
    ShaderProgram(UInt32 count, Shader* shaders[]);
    ~ShaderProgram();
    void bind();
    void unbind();
    void addUniform(const char* uniformName);
    template<typename T> void uniform(const char* uniformName, T data);
  };
}

#endif