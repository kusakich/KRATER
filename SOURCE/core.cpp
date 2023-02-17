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

      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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

    static void keyCallBack(GLFWwindow* window, SInt32 key, SInt32 scancode, SInt32 action, SInt32 mode)
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

    static void cursorPositionCallBack(GLFWwindow* window, Float64 x, Float64 y)
    {
      cursorDeltaX = x - cursorX;
      cursorDeltaY = y - cursorY;

      cursorX = x;
      cursorY = y;
    }

    static void mouseButtonCallBack(GLFWwindow* window, SInt32 button, SInt32 action, SInt32 mode)
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
      glfwSetKeyCallback(window::window, keyCallBack);
      glfwSetCursorPosCallback(window::window, cursorPositionCallBack);
      glfwSetMouseButtonCallback(window::window, mouseButtonCallBack);
      glfwSetInputMode(window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPos(window::window, 0.0,0.0);
    }

    void pollEvents()
    {
      current++;
      cursorDeltaX = 0.0;
      cursorDeltaY = 0.0;
      scroll = 0.0;
      glfwPollEvents();
    }

    glm::vec2 getCursorPosition()
    {
      return glm::vec2(cursorX, cursorY);
    }

    glm::vec2 getCursorDeltaPosition()
    {
      return glm::vec2(cursorDeltaX, cursorDeltaY);
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
};