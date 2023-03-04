#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.hpp>
#include <events.hpp>

namespace events
{

static Window* window = nullptr;
static const UInt32 MOUSE_BUTTONS = 1024;
static UInt8    keys[1032] = {0};
static UInt32   frames[1032] = {0};
static UInt32   current = 0;
static Float64  cursorX = 0.0;
static Float64  cursorY = 0.0;
static Float64  cursorDeltaX = 0.0;
static Float64  cursorDeltaY = 0.0;
static Float64  scroll = 0.0;
static bool     cursorLocked  = false;
static bool     cursorStarted = false;

static void windowSizeCallback(GLFWwindow* handle, SInt32 width, SInt32 height)
{
  window->setSize(glm::uvec2(width, height));
  glViewport(0,0, width, height);
}

static void keyCallback(GLFWwindow* handle, SInt32 key, SInt32 scancode, SInt32 action, SInt32 mode)
{
  if (action == GLFW_PRESS)
  {
    keys[key] = 1;
    frames[key] = current;
  }
  if (action == GLFW_RELEASE)
  {
    keys[key] = 0;
    frames[key] = current;
  }
}

static void cursorPositionCallback(GLFWwindow* handle, Float64 x, Float64 y)
{
  if(cursorStarted) {
    cursorDeltaX = x - cursorX;
    cursorDeltaY = y - cursorY;
  } else
    cursorStarted = true;

  cursorX = x;
  cursorY = y;
}

static void mouseButtonCallback(GLFWwindow* handle, SInt32 button, SInt32 action, SInt32 mode)
{
  if (action == GLFW_PRESS)
  {
    keys[MOUSE_BUTTONS+button] = 1;
    frames[MOUSE_BUTTONS+button] = current;
  }
  else if (action == GLFW_RELEASE)
  {
    keys[MOUSE_BUTTONS+button] = 0;
    frames[MOUSE_BUTTONS+button] = current;
  }
}

void initialize(Window* pwindow)
{
  window = pwindow;
  glfwSetWindowSizeCallback(window->handle,   windowSizeCallback);
  glfwSetKeyCallback(window->handle,          keyCallback);
  glfwSetCursorPosCallback(window->handle,    cursorPositionCallback);
  glfwSetMouseButtonCallback(window->handle,  mouseButtonCallback);
  if(cursorLocked)
    glfwSetInputMode(window->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  else
    glfwSetInputMode(window->handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetCursorPos(window->handle, window->size.x/2,window->size.y/2);
}

void pollEvents()
{
  current++;
  cursorDeltaX = 0.0;
  cursorDeltaY = 0.0;
  scroll = 0.0;
  glfwPollEvents();
}

namespace keyboard
{
  bool isPressed(Keys keycode)
  {
    if(keycode < 0 || keycode >= events::MOUSE_BUTTONS) return 0;
    return events::keys[keycode];
  }

  bool isJustPressed(Keys keycode)
  {
    if(keycode < 0 || keycode >= events::MOUSE_BUTTONS) return 0;
    return events::keys[keycode] && (events::frames[keycode] == events::current);
  }
};

namespace mouse
{
  bool isClicked(Buttons button)
  {
    return events::keys[events::MOUSE_BUTTONS+button];
  }

  bool isJustClicked(Buttons button)
  {
    return events::keys[events::MOUSE_BUTTONS+button] && (events::frames[events::MOUSE_BUTTONS+button] == events::current);
  }
}

namespace cursor
{
  glm::vec2 getPosition()
  {
    if(events::cursorStarted)
      return glm::vec2(events::cursorX, events::cursorY);
    else
      return glm::vec2(0);
  }

  glm::vec2 getDeltaPosition()
  {
    if(events::cursorStarted)
      return glm::vec2(events::cursorDeltaX, events::cursorDeltaY);
    else
      return glm::vec2(0);
  }

  void setPosition(glm::vec2 position)
  {
    glfwSetCursorPos(window->handle, position.x, position.y);
    cursorX = position.x;
    cursorY = position.y;
    cursorDeltaX = 0.0;
    cursorDeltaY = 0.0;
  }

  bool isLocked()
  {
    return events::cursorLocked;
  }

  void lock(bool lock)
  {
    events::cursorLocked = lock;
    if(events::cursorLocked)
      glfwSetInputMode(window->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
      glfwSetInputMode(window->handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }
};

};