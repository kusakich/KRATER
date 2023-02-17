#include <util.hpp>
#include <shader.hpp>

#include <glad/glad.h>

gl::Shader::Shader(const char* shaderFileName, Type ptype) : id_(0), type_(ptype), id(id_), type(type_)
{
  if(!shaderFileName) {
    spdlog::critical("Enter a shader file name!");
    return; }

  std::ifstream shaderFile(shaderFileName, std::ios::binary);
  if(!shaderFile.is_open()) {
    spdlog::critical("File not found {0}", shaderFileName);
    return; }

  char * shaderData = new char[100000];
  std::fill(shaderData, &shaderData[99999], 0);
  shaderFile.read(shaderData, 100000);

  if(type_ == Type::VERTEX)
    id_ = glCreateShader(GL_VERTEX_SHADER);
  else if(type_ == Type::FRAGMENT)
    id_ = glCreateShader(GL_FRAGMENT_SHADER);
  else
    return;

  SInt32 succesCompileShader = true;
  glShaderSource(id_, 1, &shaderData, NULL);
  delete[] shaderData;
  glCompileShader(id_);
  glGetShaderiv(id_, GL_COMPILE_STATUS, &succesCompileShader);

  if(!succesCompileShader) {
    SInt32 infoLogLength = 0;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);
    char * infoLog = new char[infoLogLength];
    glGetShaderInfoLog(id_, infoLogLength, NULL, infoLog);
    spdlog::critical(infoLog);
    delete[] infoLog;
    return; }
}

gl::Shader::~Shader()
{
}

gl::ShaderProgram::ShaderProgram(UInt32 count, Shader* shaders[])
{
  id_ = glCreateProgram();

  for(UInt32 i = 0; i < count; i++)
    glAttachShader(id_, shaders[i]->id);

  glLinkProgram(id_);

  SInt32 succesLinkProgram = true;
  glGetProgramiv(id_, GL_LINK_STATUS, &succesLinkProgram);

  if(!succesLinkProgram) {
    SInt32 infoLogLength = 0;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);
    char * infoLog = new char[infoLogLength];
    glGetProgramInfoLog(id_, infoLogLength, NULL, infoLog);
    spdlog::critical(infoLog);
    delete[] infoLog;
    return; }
}

gl::ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(id_);
}

void gl::ShaderProgram::bind()
{
  glUseProgram(id_);
}

void gl::ShaderProgram::unbind()
{
  glUseProgram(0);
}

void gl::ShaderProgram::addUniform(const char* uniformName)
{
  SInt32 location = glGetUniformLocation(id_, uniformName);
  uniforms_[uniformName] = location;
}

template<>
void gl::ShaderProgram::uniform<glm::mat4>(const char* uniformName, glm::mat4 data)
{
  glUniformMatrix4fv(uniforms_[uniformName], 1, GL_FALSE, &data[0][0]);
}

template<>
void gl::ShaderProgram::uniform<glm::mat3>(const char* uniformName, glm::mat3 data)
{
  glUniformMatrix3fv(uniforms_[uniformName], 1, GL_FALSE, &data[0][0]);
}

template<>
void gl::ShaderProgram::uniform<glm::vec4>(const char* uniformName, glm::vec4 data)
{
  glUniform4fv(uniforms_[uniformName], 1, &data[0]);
}

template<>
void gl::ShaderProgram::uniform<glm::vec3>(const char* uniformName, glm::vec3 data)
{
  glUniform3fv(uniforms_[uniformName], 1, &data[0]);
}