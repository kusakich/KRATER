#include <glad/glad.h>
#include <mini/ini.h>

#include <util.hpp>
#include <core.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <world.hpp>
#include <renderer.hpp>

int main()
{
  spdlog::info("Starting");
  bool running = true;
  bool isShowF3 = false;

  mINI::INIFile settingsFile("BASE/settings.ini");
  mINI::INIStructure settings;
  settingsFile.read(settings);

  Float64 deltaTime   = 1;
  UInt32 windowWidth  = std::stoi(settings["Window"]["width"]);
  UInt32 windowHeight = std::stoi(settings["Window"]["height"]);
  const std::string windowTitle = settings["Window"]["title"];

  core::window::create(windowTitle.c_str(), windowWidth, windowHeight);
  core::events::initialize();
  renderer::initialize();

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

  Camera mainCamera(glm::vec3(8.0,8.0,30.0), glm::vec3(0.0,0.0,0.0));
  mainCamera.setProjectionMatrix(glm::perspective(70.0,
                                                  (Float64)core::window::getWidth()/
                                                  (Float64)core::window::getHeight(),
                                                  0.1,1000.0));

  while(running)
  {
    if(core::window::isClose())
      running = false;

    if(core::keyboard::isJustPressed(core::keyboard::Keys::ESCAPE))
      running = false;

    if(core::keyboard::isJustPressed(core::keyboard::Keys::F3))
      isShowF3 = !isShowF3;

    if(core::keyboard::isJustPressed(core::keyboard::Keys::TAB))
    {
      core::cursor::lock(!core::cursor::isLocked());
      core::cursor::setPosition(glm::vec2(windowWidth/2, windowHeight/2));
    }

    core::events::pollEvents();

    deltaTime    = core::window::getDeltaTime();
    windowWidth  = core::window::getWidth();
    windowHeight = core::window::getHeight();

    mainCamera.setProjectionMatrix( glm::perspective(
                                      70.0,
                                      (Float64)windowWidth/
                                      (Float64)windowHeight,
                                      0.1,1000.0));

    if(core::keyboard::isPressed(core::keyboard::Keys::W))
      mainCamera.translate(glm::vec3( 10.0*deltaTime*glm::sin(mainCamera.rotation.z),
                                      10.0*deltaTime*glm::cos(mainCamera.rotation.z),
                                      0.0));
    if(core::keyboard::isPressed(core::keyboard::Keys::A))
      mainCamera.translate(glm::vec3(-10.0*deltaTime*glm::cos(mainCamera.rotation.z),
                                      10.0*deltaTime*glm::sin(mainCamera.rotation.z),
                                      0.0));
    if(core::keyboard::isPressed(core::keyboard::Keys::S))
      mainCamera.translate(glm::vec3(-10.0*deltaTime*glm::sin(mainCamera.rotation.z),
                                     -10.0*deltaTime*glm::cos(mainCamera.rotation.z),
                                      0.0));
    if(core::keyboard::isPressed(core::keyboard::Keys::D))
      mainCamera.translate(glm::vec3( 10.0*deltaTime*glm::cos(mainCamera.rotation.z),
                                     -10.0*deltaTime*glm::sin(mainCamera.rotation.z),
                                      0.0));

    if(core::keyboard::isPressed(core::keyboard::Keys::SPACE))
      mainCamera.translate(glm::vec3(0.0,0.0, 10.0*deltaTime));
    if(core::keyboard::isPressed(core::keyboard::Keys::LSHIFT))
      mainCamera.translate(glm::vec3(0.0,0.0,-10.0*deltaTime));

    if(core::keyboard::isPressed(core::keyboard::Keys::UP))
      mainCamera.rotateX( 3.0*deltaTime);
    if(core::keyboard::isPressed(core::keyboard::Keys::DOWN))
      mainCamera.rotateX(-3.0*deltaTime);
    if(core::keyboard::isPressed(core::keyboard::Keys::RIGHT))
      mainCamera.rotateZ( 5.0*deltaTime);
    if(core::keyboard::isPressed(core::keyboard::Keys::LEFT))
      mainCamera.rotateZ(-5.0*deltaTime);

    if(core::cursor::isLocked()) {
      mainCamera.rotateX(-(Float64)core::cursor::getDeltaPosition().y*deltaTime*1.7);
      mainCamera.rotateZ( (Float64)core::cursor::getDeltaPosition().x*deltaTime*1.7);
    }

    mainCamera.update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1);

    chunkShaderProgram->bind();
    chunkShaderProgram->uniform<glm::mat4>("m", glm::mat4(1));
    chunkShaderProgram->uniform<glm::mat4>("v", mainCamera.view);
    chunkShaderProgram->uniform<glm::mat4>("p", mainCamera.projection);
    chunkShaderProgram->uniform<glm::vec3>("l", glm::vec3(0.3,0.5,1.0));
    for(UInt32 y = 0; y < World::WIDTH; ++y)
      for(UInt32 x = 0; x < World::WIDTH; ++x)
      {
        chunkShaderProgram->uniform<glm::mat4>("m", glm::translate(glm::mat4(1),
                                                    glm::vec3(x*Chunk::WIDTH,
                                                              y*Chunk::WIDTH,
                                                              0.0)));
        chunkMeshes[y][x]->draw();
      }

    glm::vec3   select = mainCamera.raycast(0.2);
    glm::ivec3 iselect = glm::vec3((SInt32)round(select.x),
                                   (SInt32)round(select.y),
                                   (SInt32)round(select.z));

    if(core::mouse::isClicked(core::mouse::Buttons::LEFT) && core::cursor::isLocked())
    {
      world->setBlockType(iselect.x,iselect.y,iselect.z, 0);
      Chunk * currentChunk = world->getChunk(glm::uvec2(iselect.x/Chunk::WIDTH,
                                                        iselect.y/Chunk::WIDTH));
      if(currentChunk != nullptr)
      {
        currentChunk->computeLight();
        delete chunkMeshes[iselect.y/Chunk::WIDTH][iselect.x/Chunk::WIDTH];
        chunkMeshes[iselect.y/Chunk::WIDTH][iselect.x/Chunk::WIDTH] =
        renderer::renderChunk(world, currentChunk->index);
      }
    }

    if(core::mouse::isClicked(core::mouse::Buttons::RIGHT) && core::cursor::isLocked())
    {
      select = mainCamera.raycast(-0.4);
      iselect = glm::vec3((SInt32)round(select.x),
                          (SInt32)round(select.y),
                          (SInt32)round(select.z));

      world->setBlockType(iselect.x,iselect.y,iselect.z,1);
      Chunk * currentChunk = world->getChunk(glm::uvec2(iselect.x/Chunk::WIDTH,
                                                        iselect.y/Chunk::WIDTH));
      if(currentChunk != nullptr)
      {
        currentChunk->computeLight();
        delete chunkMeshes[iselect.y/Chunk::WIDTH][iselect.x/Chunk::WIDTH];
        chunkMeshes[iselect.y/Chunk::WIDTH][iselect.x/Chunk::WIDTH] =
        renderer::renderChunk(world, currentChunk->index);
      }
    }

    selectTexture->bind();
    if(core::cursor::isLocked())
    {
      renderer::drawRect(Rect(windowWidth/2-8,
                                  windowHeight/2-8,
                                  16, 16));
    } else {
      renderer::drawRect(Rect(core::cursor::getPosition().x,
                                  core::cursor::getPosition().y,
                                  16, 16));
    }

    if(isShowF3)
    {
      UInt32  fps = (UInt32)(1.0/deltaTime);
      renderer::drawText("FPS: " + std::to_string(fps),5,16,0.33);
      renderer::drawText("KRATER alpha 1",5,32,0.33);
      renderer::drawText(
        "pos  "
        + std::to_string(mainCamera.position.x).erase(std::to_string(mainCamera.position.x).size()-4) + " "
        + std::to_string(mainCamera.position.y).erase(std::to_string(mainCamera.position.y).size()-4) + " "
        + std::to_string(mainCamera.position.z).erase(std::to_string(mainCamera.position.z).size()-4),
        5,48,0.33);
      renderer::drawText(
        "rot  "
        + std::to_string(mainCamera.rotation.x).erase(std::to_string(mainCamera.rotation.x).size()-4) + " "
        + std::to_string(mainCamera.rotation.y).erase(std::to_string(mainCamera.rotation.y).size()-4) + " "
        + std::to_string(mainCamera.rotation.z).erase(std::to_string(mainCamera.rotation.z).size()-4),
        5,64,0.33);
      renderer::drawText("block "
        + std::to_string(iselect.x) + " "
        + std::to_string(iselect.y) + " "
        + std::to_string(iselect.z),
        5,80,0.33);
    }

    core::window::swapBuffers();
  }

  renderer::finalize();
  core::window::destroy();
}