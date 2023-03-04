#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <util.hpp>
#include <window.hpp>

Window::Window(std::string ptitle, UInt32 pwidth, UInt32 pheight) :
title_(ptitle),
size_(pwidth, pheight),
deltaTime_(1.0),
handle(handle_),
title(title_),
size(size_),
deltaTime(deltaTime_)
{
  spdlog::info("Creating window...");
  if(!glfwInit()) {
    spdlog::critical("Failed GLFW initialized");
    return; }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  handle_ = glfwCreateWindow(pwidth, pheight, ptitle.c_str(), nullptr, nullptr);

  if(!handle_) {
    spdlog::critical("Failed window created");
    glfwTerminate();
    return; }

  glfwMakeContextCurrent(handle_);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::critical("GLAD Loader error!");
    glfwTerminate();
    return; }

  glViewport(0,0, pwidth, pheight);
  glfwSwapInterval(0);

  spdlog::info("Creating window finished");
}

Window::~Window()
{
  glfwDestroyWindow(handle_);
  glfwTerminate();
}

void Window::swapBuffers()
{
  static Float64 lastTime;
  Float64 currentTime = glfwGetTime();
  deltaTime_ = currentTime - lastTime;
  lastTime = currentTime;
  glfwSwapBuffers(handle_);
}

bool Window::isClose()
{
  return glfwWindowShouldClose(handle_);
}

void Window::setSize(glm::uvec2 psize)
{
  size_ = psize;
}