#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <util.hpp>

struct GLFWwindow;

class Window
{
private:
  GLFWwindow* handle_;
  std::string title_;
  glm::uvec2  size_;
  Float64     delta_;
public:
  GLFWwindow* const & handle;
  std::string const & title;
  glm::uvec2  const & size;
  Float64     const & delta;
  Window( const std::string ptitle  = "Default",
          const UInt32      pwidth  = 800,
          const UInt32      pheight = 600);
  ~Window();

  void swapBuffers();
  bool isClose();

  void setSize(glm::uvec2 psize);
};

#endif