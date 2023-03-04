#include <glad/glad.h>
#include <mini/ini.h>

#include <util.hpp>
#include <window.hpp>
#include <events.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <world.hpp>
#include <renderer.hpp>

int main()
{
  spdlog::info("Starting");
  bool running  = true;
  bool isShowF3 = false;

  mINI::INIFile settingsFile("BASE/settings.ini");
  mINI::INIStructure settings;
  settingsFile.read(settings);

  Float32 deltaTime   = 1.0;
  UInt32 windowWidth  = std::stoi(settings["Graphics"]["width"]);
  UInt32 windowHeight = std::stoi(settings["Graphics"]["height"]);
  const std::string windowTitle = settings["Graphics"]["title"];

  Window* window = new Window(windowTitle, windowWidth, windowHeight);
  events::initialize(window);
  renderer::initialize(window, settings["Graphics"]["font"]);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CW);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gl::Shader* chunkShaders[2];
  chunkShaders[0] = new gl::Shader("BASE/SHADERS/chunk.vert", gl::Shader::Type::VERTEX);
  chunkShaders[1] = new gl::Shader("BASE/SHADERS/chunk.frag", gl::Shader::Type::FRAGMENT);
  gl::ShaderProgram* chunkShaderProgram = new gl::ShaderProgram(2, chunkShaders);
  chunkShaderProgram->addUniform("m");
  chunkShaderProgram->addUniform("v");
  chunkShaderProgram->addUniform("p");
  chunkShaderProgram->addUniform("l");

  World* world = new World();
  gl::Mesh* chunkMeshes[World::WIDTH][World::WIDTH];
  for(UInt32 y = 0; y < World::WIDTH; ++y)
    for(UInt32 x = 0; x < World::WIDTH; ++x)
    {
      chunkMeshes[y][x] = renderer::renderChunk(world, glm::uvec2(x,y));
    }

  gl::Texture* selectTexture = new gl::Texture("BASE/GUI/select.png");

  Camera* camera = new Camera(glm::vec3(8.0,8.0,30.0), glm::vec3(0.0,0.0,0.0));
  camera->setProjectionMatrix(glm::perspective(70.0,
                                               (Float64)window->size.x/
                                               (Float64)window->size.y,
                                               0.1,1000.0));

  while(running)
  {
    if(window->isClose())
      running = false;

    if(events::keyboard::isJustPressed(events::keyboard::Keys::ESCAPE))
      running = false;

    if(events::keyboard::isJustPressed(events::keyboard::Keys::F3))
      isShowF3 = !isShowF3;

    if(events::keyboard::isJustPressed(events::keyboard::Keys::TAB))
    {
      events::cursor::lock(!events::cursor::isLocked());
      events::cursor::setPosition(glm::vec2(window->size.x/2, window->size.y/2));
    }

    events::pollEvents();

    deltaTime = window->deltaTime;

    camera->setProjectionMatrix(glm::perspective(
                                70.0,
                                (Float64)window->size.x/
                                (Float64)window->size.y,
                                0.1,1000.0));

    if(events::keyboard::isPressed(events::keyboard::Keys::W))
      camera->translate(glm::vec3(10.0*deltaTime*glm::sin(camera->rotation.z),
                                  10.0*deltaTime*glm::cos(camera->rotation.z),
                                  0.0));
    if(events::keyboard::isPressed(events::keyboard::Keys::A))
      camera->translate(glm::vec3(-10.0*deltaTime*glm::cos(camera->rotation.z),
                                   10.0*deltaTime*glm::sin(camera->rotation.z),
                                   0.0));
    if(events::keyboard::isPressed(events::keyboard::Keys::S))
      camera->translate(glm::vec3(-10.0*deltaTime*glm::sin(camera->rotation.z),
                                  -10.0*deltaTime*glm::cos(camera->rotation.z),
                                   0.0));
    if(events::keyboard::isPressed(events::keyboard::Keys::D))
      camera->translate(glm::vec3( 10.0*deltaTime*glm::cos(camera->rotation.z),
                                  -10.0*deltaTime*glm::sin(camera->rotation.z),
                                   0.0));

    if(events::keyboard::isPressed(events::keyboard::Keys::SPACE))
      camera->translate(glm::vec3(0.0,0.0, 10.0*deltaTime));
    if(events::keyboard::isPressed(events::keyboard::Keys::LSHIFT))
      camera->translate(glm::vec3(0.0,0.0,-10.0*deltaTime));

    if(events::keyboard::isPressed(events::keyboard::Keys::UP))
      camera->rotateX( 3.0*deltaTime);
    if(events::keyboard::isPressed(events::keyboard::Keys::DOWN))
      camera->rotateX(-3.0*deltaTime);
    if(events::keyboard::isPressed(events::keyboard::Keys::RIGHT))
      camera->rotateZ( 5.0*deltaTime);
    if(events::keyboard::isPressed(events::keyboard::Keys::LEFT))
      camera->rotateZ(-5.0*deltaTime);

    if(events::cursor::isLocked()) {
      camera->rotateX(-(Float64)events::cursor::getDeltaPosition().y*deltaTime*1.7);
      camera->rotateZ( (Float64)events::cursor::getDeltaPosition().x*deltaTime*1.7);
    }

    camera->update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1);

    chunkShaderProgram->bind();
    chunkShaderProgram->uniform<glm::mat4>("m", glm::mat4(1));
    chunkShaderProgram->uniform<glm::mat4>("v", camera->view);
    chunkShaderProgram->uniform<glm::mat4>("p", camera->projection);
    chunkShaderProgram->uniform<glm::vec3>("l", glm::vec3(0.3,0.5,1.0));
    for(UInt32 y = 0; y < World::WIDTH; ++y)
      for(UInt32 x = 0; x < World::WIDTH; ++x)
      {
        chunkMeshes[y][x]->draw();
      }

    glm::vec3   select = renderer::raycast(camera, 0.2);
    glm::ivec3 iselect = glm::vec3((SInt32)round(select.x),
                                   (SInt32)round(select.y),
                                   (SInt32)round(select.z));

    if(events::mouse::isClicked(events::mouse::Buttons::LEFT) && events::cursor::isLocked())
    {
      world->setBlockType(iselect.x,iselect.y,iselect.z, 0);
    }

    if(events::mouse::isClicked(events::mouse::Buttons::RIGHT) && events::cursor::isLocked())
    {
      select  = renderer::raycast(camera, -0.4);
      iselect = glm::vec3((SInt32)round(select.x),
                          (SInt32)round(select.y),
                          (SInt32)round(select.z));

      world->setBlockType(iselect.x,iselect.y,iselect.z,1);
    }

    if(world->edited)
    {
      for(UInt32 y = 0; y < World::WIDTH; ++y)
      for(UInt32 x = 0; x < World::WIDTH; ++x)
      {
        Chunk * currentChunk = world->getChunk(glm::uvec2(x,y));
        if(currentChunk != nullptr && currentChunk->edited)
        {
          currentChunk->computeLight();
          delete chunkMeshes[y][x];
          chunkMeshes[y][x] = renderer::renderChunk(world, currentChunk->index);
          currentChunk->edited = false;
        }
      }
      world->edited = false;
    }

    selectTexture->bind();
    if(events::cursor::isLocked())
    {
      renderer::drawRect(Rect(window->size.x/2-8,
                              window->size.y/2-8,
                              16, 16));
    } else {
      renderer::drawRect(Rect(events::cursor::getPosition().x,
                              events::cursor::getPosition().y,
                              16, 16));
    }

    if(isShowF3)
    {
      UInt32  fps = (UInt32)(1.0/deltaTime);
      renderer::drawText("FPS: " + std::to_string(fps),5,16,1);
      renderer::drawText("KRATER alpha 1",5,32,1);
      renderer::drawText("XYZ  "
        + std::to_string(camera->position.x).erase(std::to_string(camera->position.x).size()-4) + " "
        + std::to_string(camera->position.y).erase(std::to_string(camera->position.y).size()-4) + " "
        + std::to_string(camera->position.z).erase(std::to_string(camera->position.z).size()-4),
        5,48,1);
      renderer::drawText("rotation  "
        + std::to_string(camera->rotation.x).erase(std::to_string(camera->rotation.x).size()-4) + " "
        + std::to_string(camera->rotation.y).erase(std::to_string(camera->rotation.y).size()-4) + " "
        + std::to_string(camera->rotation.z).erase(std::to_string(camera->rotation.z).size()-4),
        5,64,1);
      renderer::drawText("block "
        + std::to_string(iselect.x) + " "
        + std::to_string(iselect.y) + " "
        + std::to_string(iselect.z),
        5,80,1);
    }

    window->swapBuffers();
  }

  renderer::finalize();
  delete window;
}