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
  bool isShowF3 = true;

  mINI::INIFile settingsFile("BASE/settings.ini");
  mINI::INIStructure settings;
  settingsFile.read(settings);

  Float64 deltaTime   = 1;
  UInt32 windowWidth  = std::stoi(settings["Window"]["width"]);
  UInt32 windowHeight = std::stoi(settings["Window"]["height"]);
  const std::string windowTitle = settings["Window"]["title"];

  core::window::create(windowTitle.c_str(), windowWidth, windowHeight);
  core::events::initialize();
  gl::renderer::initialize();

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
      chunkMeshes[y][x] = gl::renderer::renderObject<ChunkData>(world->getChunk(x,y)->getData());
    }

  gl::Texture* selectTexture = new gl::Texture("BASE/GUI/select.png");

  Camera mainCamera(glm::vec3(8.0,8.0,30.0), glm::vec3(0.0,0.0,0.0));
  mainCamera.setProjectionMatrix(glm::perspective(70.0,
                                                  (Float64)core::window::getWidth()/
                                                  (Float64)core::window::getHeight(),
                                                  0.1,100.0));

  while(running)
  {
    if(core::window::isClose())
      running = false;

    if(core::keyboard::isJustPressed(core::keyboard::Keys::ESCAPE))
      running = false;

    if(core::keyboard::isJustPressed(core::keyboard::Keys::F3))
      isShowF3 = !isShowF3;

    core::events::pollEvents();

    deltaTime    = core::window::getDeltaTime();
    windowWidth  = core::window::getWidth();
    windowHeight = core::window::getHeight();

    mainCamera.setProjectionMatrix( glm::perspective(
                                      70.0,
                                      (Float64)windowWidth/
                                      (Float64)windowHeight,
                                      0.1,100.0));

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

    if(core::keyboard::isPressed(core::keyboard::Keys::NUMPAD_8))
      mainCamera.rotateX( 3.0*deltaTime);
    if(core::keyboard::isPressed(core::keyboard::Keys::NUMPAD_2))
      mainCamera.rotateX(-3.0*deltaTime);
    if(core::keyboard::isPressed(core::keyboard::Keys::NUMPAD_6))
      mainCamera.rotateZ( 5.0*deltaTime);
    if(core::keyboard::isPressed(core::keyboard::Keys::NUMPAD_4))
      mainCamera.rotateZ(-5.0*deltaTime);

    mainCamera.rotateX(-(Float64)core::events::getCursorDeltaPosition().y*deltaTime*1.7);
    mainCamera.rotateZ( (Float64)core::events::getCursorDeltaPosition().x*deltaTime*1.7);

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
        chunkShaderProgram->uniform<glm::mat4>("m", glm::translate(glm::mat4(1), glm::vec3(x*Chunk::WIDTH,y*Chunk::WIDTH,0.0)));
        chunkMeshes[y][x]->draw();
      }

    glm::vec3   select = mainCamera.raycast(0.2);
    glm::ivec3 iselect = glm::vec3((SInt32)round(select.x),
                                   (SInt32)round(select.y),
                                   (SInt32)round(select.z));

    //if(core::mouse::isClicked(core::mouse::Buttons::LEFT))
    //{
    //  if( iselect.x >= 0 && iselect.x < Chunk::WIDTH &&
    //      iselect.y >= 0 && iselect.y < Chunk::WIDTH &&
    //      iselect.z >= 0 && iselect.z < Chunk::WIDTH &&
    //      chunk.data3[iselect.z][iselect.y][iselect.x].type != 0)
    //  {
    //    chunk.data3[iselect.z][iselect.y][iselect.x].type = 0;
    //    chunk.computeLight();
    //    delete chunkMesh;
    //    chunkMesh = gl::renderer::renderObject<Chunk>(chunk);
    //  }
    //}
//
    //if(core::mouse::isClicked(core::mouse::Buttons::RIGHT))
    //{
    //  select = mainCamera.raycast(-0.4);
    //  iselect = glm::vec3((SInt32)round(select.x),
    //                      (SInt32)round(select.y),
    //                      (SInt32)round(select.z));
//
    //  if( iselect.x >= 0 && iselect.x < Chunk::WIDTH &&
    //      iselect.y >= 0 && iselect.y < Chunk::WIDTH &&
    //      iselect.z >= 0 && iselect.z < Chunk::WIDTH &&
    //      chunk.data3[iselect.z][iselect.y][iselect.x].type == 0)
    //  {
    //    chunk.data3[iselect.z][iselect.y][iselect.x].type = 1;
    //    chunk.computeLight();
    //    delete chunkMesh;
    //    chunkMesh = gl::renderer::renderObject<Chunk>(chunk);
    //  }
    //}

    selectTexture->bind();
    gl::renderer::drawRect(Rect(windowWidth/2-8,
                                windowHeight/2-8,
                                16, 16));

    if(isShowF3)
    {
      UInt32  fps = (UInt32)(1.0/deltaTime);
      gl::renderer::drawText("FPS: " + std::to_string(fps),3,16,0.33);
      gl::renderer::drawText("KRATER alpha 1",3,32,0.33);
    }

    core::window::swapBuffers();
  }

  gl::renderer::finalize();
  core::window::destroy();
}