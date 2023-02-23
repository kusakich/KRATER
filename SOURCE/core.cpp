#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <util.hpp>
#include <core.hpp>

namespace core
{
  namespace window
  {
    static GLFWwindow*  window;
    static UInt32       width;
    static UInt32       height;
    static Float64      deltaTime;

    void create(const char* ptitle, const UInt32 pwidth, const UInt32 pheight)
    {
      spdlog::info("Creating window...");
      if(!glfwInit()) {
        spdlog::critical("Failed GLFW initialized");
        return; }

      width  = pwidth;
      height = pheight;

      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
      glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
      glfwWindowHint(GLFW_DEPTH_BITS, 24);
      glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      window = glfwCreateWindow(width, height, ptitle, nullptr, nullptr);

      if(!window) {
        spdlog::critical("Failed window created");
        glfwTerminate();
        return; }

      glfwMakeContextCurrent(window);
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::critical("GLAD Loader error!");
        glfwTerminate();
        return; }

      glViewport(0,0, width, height);
      glfwSwapInterval(0);

      spdlog::info("Creating window finished");
    }
    void destroy()
    {
      glfwDestroyWindow(window);
      glfwTerminate();
    }
    void swapBuffers()
    {
      static Float64 lastTime;
      Float64 currentTime = glfwGetTime();
      deltaTime = currentTime - lastTime;
      lastTime = currentTime;
      glfwSwapBuffers(window);
    }

    bool isClose()
    {
      return glfwWindowShouldClose(window);
    }

    UInt32 getWidth()
    {
      return width;
    }

    UInt32 getHeight()
    {
      return height;
    }

    Float64 getDeltaTime()
    {
      return deltaTime;
    }
  };

  namespace events
  {
    const  UInt32   MOUSE_BUTTONS = 1024;
    static UInt8    keys[1032] = {0};
    static UInt32   frames[1032] = {0};
    static UInt32   current = 0;
    static Float64  cursorX = 0.0;
    static Float64  cursorY = 0.0;
    static Float64  cursorDeltaX = 0.0;
    static Float64  cursorDeltaY = 0.0;
    static Float64  scroll = 0.0;
    static bool     cursorLocked  = true;
    static bool     cursorStarted = false;

    void windowSizeCallback(GLFWwindow* window, SInt32 width, SInt32 height)
    {
      window::width = width;
      window::height = height;
      glViewport(0,0, width, height);
    }

    static void keyCallback(GLFWwindow* window, SInt32 key, SInt32 scancode, SInt32 action, SInt32 mode)
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

    static void cursorPositionCallback(GLFWwindow* window, Float64 x, Float64 y)
    {
      if(cursorStarted) {
        cursorDeltaX = x - cursorX;
        cursorDeltaY = y - cursorY;
      } else
        cursorStarted = true;

      cursorX = x;
      cursorY = y;
    }

    static void mouseButtonCallback(GLFWwindow* window, SInt32 button, SInt32 action, SInt32 mode)
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

    void initialize()
    {
      glfwSetWindowSizeCallback(window::window, windowSizeCallback);
      glfwSetKeyCallback(window::window, keyCallback);
      glfwSetCursorPosCallback(window::window, cursorPositionCallback);
      glfwSetMouseButtonCallback(window::window, mouseButtonCallback);
      if(cursorLocked)
        glfwSetInputMode(window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      else
        glfwSetInputMode(window::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      glfwSetCursorPos(window::window, window::width/2,window::height/2);
    }

    void pollEvents()
    {
      current++;
      cursorDeltaX = 0.0;
      cursorDeltaY = 0.0;
      scroll = 0.0;
      glfwPollEvents();
    }
  };

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
      glfwSetCursorPos(window::window, position.x, position.y);
    }

    bool isLocked()
    {
      return events::cursorLocked;
    }

    void lock(bool lock)
    {
      events::cursorLocked = lock;
      if(events::cursorLocked)
        glfwSetInputMode(window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      else
        glfwSetInputMode(window::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
  }
};