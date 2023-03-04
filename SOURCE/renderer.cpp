#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <util.hpp>
#include <window.hpp>
#include <camera.hpp>
#include <primitives.hpp>
#include <world.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <marchingCubesTable.hpp>

static const float basicRectVertex[] =
{
  0.0,0.0, 0.0,0.0,
  0.0,1.0, 0.0,1.0,
  1.0,1.0, 1.0,1.0,
  1.0,1.0, 1.0,1.0,
  1.0,0.0, 1.0,0.0,
  0.0,0.0, 0.0,0.0
};

static const float basicQuadVertex[] =
{
  /*+Z  vertex      normal     s    t*/
 -0.5,-0.5, 0.5, 0.0,0.0,0.0, 0.0, 1.0,
  0.5,-0.5, 0.5, 0.0,0.0,0.0, 1.0, 1.0,
  0.5, 0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
  0.5, 0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
 -0.5, 0.5, 0.5, 0.0,0.0,0.0, 0.0, 0.0,
 -0.5,-0.5, 0.5, 0.0,0.0,0.0, 0.0, 1.0,
  /*-Z*/
 -0.5, 0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
  0.5, 0.5,-0.5, 0.0,0.0,0.0, 1.0, 1.0,
  0.5,-0.5,-0.5, 0.0,0.0,0.0, 1.0, 0.0,
  0.5,-0.5,-0.5, 0.0,0.0,0.0, 1.0, 0.0,
 -0.5,-0.5,-0.5, 0.0,0.0,0.0, 0.0, 0.0,
 -0.5, 0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
  /*+Y*/
  0.5, 0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
 -0.5, 0.5,-0.5, 0.0,0.0,0.0, 1.0, 1.0,
 -0.5, 0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
 -0.5, 0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
  0.5, 0.5, 0.5, 0.0,0.0,0.0, 0.0, 0.0,
  0.5, 0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
  /*-Y*/
 -0.5,-0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
  0.5,-0.5,-0.5, 0.0,0.0,0.0, 1.0, 1.0,
  0.5,-0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
  0.5,-0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
 -0.5,-0.5, 0.5, 0.0,0.0,0.0, 0.0, 0.0,
 -0.5,-0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
  /*+X*/
  0.5,-0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
  0.5, 0.5,-0.5, 0.0,0.0,0.0, 1.0, 1.0,
  0.5, 0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
  0.5, 0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
  0.5,-0.5, 0.5, 0.0,0.0,0.0, 0.0, 0.0,
  0.5,-0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
  /*-X*/
 -0.5, 0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0,
 -0.5,-0.5,-0.5, 0.0,0.0,0.0, 1.0, 1.0,
 -0.5,-0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
 -0.5,-0.5, 0.5, 0.0,0.0,0.0, 1.0, 0.0,
 -0.5, 0.5, 0.5, 0.0,0.0,0.0, 0.0, 0.0,
 -0.5, 0.5,-0.5, 0.0,0.0,0.0, 0.0, 1.0
};

namespace renderer
{
  static Window*            window                 = nullptr;

  static gl::Mesh*          basicRectMesh          = nullptr;
  static gl::ShaderProgram* basicRectShaderProgram = nullptr;

  static gl::Mesh*          basicQuadMesh          = nullptr;
  static gl::ShaderProgram* basicQuadShaderProgram = nullptr;

  static gl::ShaderProgram* basicTextShaderProgram = nullptr;

  struct Character
  {
    UInt32     texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    SInt64     advance;
  };
  static std::map<char, Character> characters;

  void initialize(Window* pwindow, std::string fontFile);
  void finalize();
  void drawQuad(Quad quad, Camera camera);
  void drawRect(Rect rect);
  void drawText(std::string text, Float32 x, Float32 y, Float32 scale);

  gl::Mesh* renderChunk(World* world, glm::uvec2 chunkIndex);

  glm::vec3 raycast(Camera* camera, Float32 delta);
};


void renderer::initialize(Window* pwindow, std::string fontFile)
{
  window = pwindow;
  if(!window)
  {
    spdlog::critical("Failed window initialize in renderer");
    return;
  }

  ///////////RECT///////////
  basicRectMesh = new gl::Mesh(basicRectVertex, 6, (UInt32[]){2,2,0}, GL_TRIANGLES);
  gl::Shader* basicRectShaders[2];
  basicRectShaders[0] = new gl::Shader("BASE/SHADERS/rect.vert", gl::Shader::Type::VERTEX);
  basicRectShaders[1] = new gl::Shader("BASE/SHADERS/rect.frag", gl::Shader::Type::FRAGMENT);
  basicRectShaderProgram = new gl::ShaderProgram(2, basicRectShaders);
  basicRectShaderProgram->addUniform("m");
  basicRectShaderProgram->addUniform("v");

  ///////////QUAD///////////
  basicQuadMesh = new gl::Mesh(basicQuadVertex, 36, (UInt32[]){3,3,2,0}, GL_TRIANGLES);
  gl::Shader* basicQuadShaders[2];
  basicQuadShaders[0] = new gl::Shader("BASE/SHADERS/quad.vert", gl::Shader::Type::VERTEX);
  basicQuadShaders[1] = new gl::Shader("BASE/SHADERS/quad.frag", gl::Shader::Type::FRAGMENT);
  basicQuadShaderProgram = new gl::ShaderProgram(2, basicQuadShaders);
  basicQuadShaderProgram->addUniform("m");
  basicQuadShaderProgram->addUniform("v");
  basicQuadShaderProgram->addUniform("p");

  ///////////TEXT///////////
  gl::Shader* basicTextShaders[2];
  basicTextShaders[0] = new gl::Shader("BASE/SHADERS/text.vert", gl::Shader::Type::VERTEX);
  basicTextShaders[1] = new gl::Shader("BASE/SHADERS/text.frag", gl::Shader::Type::FRAGMENT);
  basicTextShaderProgram = new gl::ShaderProgram(2, basicTextShaders);
  basicTextShaderProgram->addUniform("m");
  basicTextShaderProgram->addUniform("v");
  basicTextShaderProgram->addUniform("color");

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    spdlog::critical("ERROR::FREETYPE: Could not init FreeType Library");
    return; }
  FT_Face face;
  if (FT_New_Face(ft, fontFile.c_str(), 0, &face)) {
    spdlog::critical("ERROR::FREETYPE: Failed to load font");
    return; }

  FT_Set_Pixel_Sizes(face, 0, 16);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (UInt32 c = 0; c < 128; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      spdlog::warn("ERROR::FREETYTPE: Failed to load Glyph");
      continue;
    }
    UInt32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Character character =
    {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top),
      face->glyph->advance.x
    };
    characters.insert(std::pair<char, Character>(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void renderer::finalize()
{
  delete basicRectMesh;
  delete basicRectShaderProgram;
  delete basicQuadMesh;
  delete basicQuadShaderProgram;
  delete basicTextShaderProgram;
}

void renderer::drawRect(Rect rect)
{
  basicRectShaderProgram->bind();
  glm::mat4 rectModel =
  { rect.size.x, 0.0, 0.0, 0.0,
    0.0, rect.size.y, 0.0, 0.0,
    0.0,         0.0, 1.0, 0.0,
    rect.position.x,
    rect.position.y,  0.0, 1.0};
  glm::mat4 rectView  = glm::ortho(0.0f, (Float32)window->size.x, (Float32)window->size.y, 0.0f);
  basicRectShaderProgram->uniform<glm::mat4>("m", rectModel);
  basicRectShaderProgram->uniform<glm::mat4>("v", rectView);
  basicRectMesh->draw();
}

void renderer::drawQuad(Quad quad, Camera camera)
{
  basicQuadShaderProgram->bind();
  glm::mat4 quadModel=
  { quad.size.x, 0.0, 0.0, 0.0,
    0.0, quad.size.y, 0.0, 0.0,
    0.0, 0.0, quad.size.z, 0.0,
    quad.position.x,
    quad.position.y,
    quad.position.z, 1.0};
  basicQuadShaderProgram->uniform<glm::mat4>("m", quadModel);
  basicQuadShaderProgram->uniform<glm::mat4>("v", camera.view);
  basicQuadShaderProgram->uniform<glm::mat4>("p", camera.projection);
  basicQuadMesh->draw();
}

void renderer::drawText(std::string text, Float32 x, Float32 y, Float32 scale)
{
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character character = characters[*c];

    glBindTexture(GL_TEXTURE_2D, character.texture);
    basicTextShaderProgram->bind();

    Rect rect(x,
              y-(Float32)character.bearing.y*scale,
              (Float32)character.size.x*scale,
              (Float32)character.size.y*scale);

    glm::mat4 rectModel =
    { rect.size.x, 0.0, 0.0, 0.0,
      0.0, rect.size.y, 0.0, 0.0,
      0.0,         0.0, 1.0, 0.0,
      rect.position.x,
      rect.position.y,  0.0, 1.0};
    glm::mat4 rectView  = glm::ortho(
                           0.0f,
                          (Float32)window->size.x,
                          (Float32)window->size.y,
                           0.0f);
    basicTextShaderProgram->uniform<glm::mat4>("m", rectModel);
    basicTextShaderProgram->uniform<glm::mat4>("v", rectView);
    basicTextShaderProgram->uniform<glm::vec3>("color", glm::vec3(1.0));
    basicRectMesh->draw();
    x += (character.advance >> 6)*scale;
  }
}

gl::Mesh* renderer::renderChunk(World* world, glm::uvec2 chunkIndex)
{
  if(world == nullptr)
    return nullptr;

  Chunk* chunk  = world->getChunk(chunkIndex);
  Chunk* chunk2 = world->getChunk(glm::uvec2(chunkIndex.x+1,chunkIndex.y));
  Chunk* chunk3 = world->getChunk(glm::uvec2(chunkIndex.x,chunkIndex.y+1));
  Chunk* chunk4 = world->getChunk(glm::uvec2(chunkIndex.x+1,chunkIndex.y+1));

  if(chunk == nullptr)
    return nullptr;

  const   UInt32  vertexSize = 7;
  static  Float32 vertexBuffer[300000] = {0};
  std::fill(vertexBuffer, &vertexBuffer[299999], 0);
  UInt32  vertexCount = 0;
  for(UInt32 z = 0; z < Chunk::WIDTH-1; z++) {
    for(UInt32 y = 0; y < Chunk::WIDTH; y++) {
      for(UInt32 x = 0; x < Chunk::WIDTH; x++) {
        UInt8 index = 0;
        if(x < Chunk::WIDTH-1 && y < Chunk::WIDTH-1)
        {
          index = index | ((1 << 0) & ((!!chunk->blocks[ z ][y+1][ x ].type) << 0));
          index = index | ((1 << 1) & ((!!chunk->blocks[ z ][y+1][x+1].type) << 1));
          index = index | ((1 << 2) & ((!!chunk->blocks[ z ][ y ][x+1].type) << 2));
          index = index | ((1 << 3) & ((!!chunk->blocks[ z ][ y ][ x ].type) << 3));
          index = index | ((1 << 4) & ((!!chunk->blocks[z+1][y+1][ x ].type) << 4));
          index = index | ((1 << 5) & ((!!chunk->blocks[z+1][y+1][x+1].type) << 5));
          index = index | ((1 << 6) & ((!!chunk->blocks[z+1][ y ][x+1].type) << 6));
          index = index | ((1 << 7) & ((!!chunk->blocks[z+1][ y ][ x ].type) << 7));
        } else
        if(x == Chunk::WIDTH-1 && y < Chunk::WIDTH-1 && chunk2 != nullptr)
        {
          index = index | ((1 << 0) & ((!!chunk ->blocks[ z ][y+1][ x ].type) << 0));
          index = index | ((1 << 1) & ((!!chunk2->blocks[ z ][y+1][ 0 ].type) << 1));
          index = index | ((1 << 2) & ((!!chunk2->blocks[ z ][ y ][ 0 ].type) << 2));
          index = index | ((1 << 3) & ((!!chunk ->blocks[ z ][ y ][ x ].type) << 3));
          index = index | ((1 << 4) & ((!!chunk ->blocks[z+1][y+1][ x ].type) << 4));
          index = index | ((1 << 5) & ((!!chunk2->blocks[z+1][y+1][ 0 ].type) << 5));
          index = index | ((1 << 6) & ((!!chunk2->blocks[z+1][ y ][ 0 ].type) << 6));
          index = index | ((1 << 7) & ((!!chunk ->blocks[z+1][ y ][ x ].type) << 7));
        } else
        if(x < Chunk::WIDTH-1 && y == Chunk::WIDTH-1 && chunk3 != nullptr)
        {
          index = index | ((1 << 0) & ((!!chunk3->blocks[ z ][ 0 ][ x ].type) << 0));
          index = index | ((1 << 1) & ((!!chunk3->blocks[ z ][ 0 ][x+1].type) << 1));
          index = index | ((1 << 2) & ((!!chunk ->blocks[ z ][ y ][x+1].type) << 2));
          index = index | ((1 << 3) & ((!!chunk ->blocks[ z ][ y ][ x ].type) << 3));
          index = index | ((1 << 4) & ((!!chunk3->blocks[z+1][ 0 ][ x ].type) << 4));
          index = index | ((1 << 5) & ((!!chunk3->blocks[z+1][ 0 ][x+1].type) << 5));
          index = index | ((1 << 6) & ((!!chunk ->blocks[z+1][ y ][x+1].type) << 6));
          index = index | ((1 << 7) & ((!!chunk ->blocks[z+1][ y ][ x ].type) << 7));
        } else
        if(x == Chunk::WIDTH-1 && y == Chunk::WIDTH-1 &&
           chunk2 != nullptr && chunk3 != nullptr && chunk4 != nullptr)
        {
          index = index | ((1 << 0) & ((!!chunk3->blocks[ z ][ 0 ][ x ].type) << 0));
          index = index | ((1 << 1) & ((!!chunk4->blocks[ z ][ 0 ][ 0 ].type) << 1));
          index = index | ((1 << 2) & ((!!chunk2->blocks[ z ][ y ][ 0 ].type) << 2));
          index = index | ((1 << 3) & ((!!chunk ->blocks[ z ][ y ][ x ].type) << 3));
          index = index | ((1 << 4) & ((!!chunk3->blocks[z+1][ 0 ][ x ].type) << 4));
          index = index | ((1 << 5) & ((!!chunk4->blocks[z+1][ 0 ][ 0 ].type) << 5));
          index = index | ((1 << 6) & ((!!chunk2->blocks[z+1][ y ][ 0 ].type) << 6));
          index = index | ((1 << 7) & ((!!chunk ->blocks[z+1][ y ][ x ].type) << 7));
        }

        if((!index) || (index==255))
          continue;

        for(UInt32 i = 0; i < 16; i++)
        {
          UInt32 vertex = trianglesTable[index][i];
          if(vertex < 0 || vertex > 11)
            break;

          vertexBuffer[vertexCount*vertexSize+0] = chunkIndex.x*Chunk::WIDTH+
                                                   (Float32)x +
                                                   pointsTable[vertex][0];
          vertexBuffer[vertexCount*vertexSize+1] = chunkIndex.y*Chunk::WIDTH+
                                                   (Float32)y +
                                                   pointsTable[vertex][1];
          vertexBuffer[vertexCount*vertexSize+2] = (Float32)z +
                                                   pointsTable[vertex][2];

          Float32 light =
          ((Float32)(world->getBlockLight(
            chunkIndex.x*Chunk::WIDTH+x+lightTable[vertex][0],
            chunkIndex.y*Chunk::WIDTH+y+lightTable[vertex][1],
            z+lightTable[vertex][2]))+
           (Float32)(world->getBlockLight(
            chunkIndex.x*Chunk::WIDTH+x+lightTable[vertex][3],
            chunkIndex.y*Chunk::WIDTH+y+lightTable[vertex][4],
            z+lightTable[vertex][5])))/
          2.0/(Float32)Block::MAX_LIGHT;

          vertexBuffer[vertexCount*vertexSize+6] = light;

          vertexCount++;

          if(vertexCount != 0 && vertexCount%3 == 0)
          {
            glm::vec3 a(vertexBuffer[(vertexCount-3)*vertexSize+0],
                        vertexBuffer[(vertexCount-3)*vertexSize+1],
                        vertexBuffer[(vertexCount-3)*vertexSize+2]);
            glm::vec3 b(vertexBuffer[(vertexCount-2)*vertexSize+0],
                        vertexBuffer[(vertexCount-2)*vertexSize+1],
                        vertexBuffer[(vertexCount-2)*vertexSize+2]);
            glm::vec3 c(vertexBuffer[(vertexCount-1)*vertexSize+0],
                        vertexBuffer[(vertexCount-1)*vertexSize+1],
                        vertexBuffer[(vertexCount-1)*vertexSize+2]);

            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;

            glm::vec3 normal = glm::cross(ab, ac);
            normal = glm::normalize(normal);

            vertexBuffer[(vertexCount-3)*vertexSize+3] = normal.x;
            vertexBuffer[(vertexCount-3)*vertexSize+4] = normal.y;
            vertexBuffer[(vertexCount-3)*vertexSize+5] = normal.z;
            vertexBuffer[(vertexCount-2)*vertexSize+3] = normal.x;
            vertexBuffer[(vertexCount-2)*vertexSize+4] = normal.y;
            vertexBuffer[(vertexCount-2)*vertexSize+5] = normal.z;
            vertexBuffer[(vertexCount-1)*vertexSize+3] = normal.x;
            vertexBuffer[(vertexCount-1)*vertexSize+4] = normal.y;
            vertexBuffer[(vertexCount-1)*vertexSize+5] = normal.z;
          }
        }
      }
    }
  }

  gl::Mesh* mesh = new gl::Mesh(vertexBuffer, vertexCount, (UInt32[]){3,3,1,0}, GL_TRIANGLES);
  return mesh;
}

glm::vec3 renderer::raycast(Camera* camera, Float32 delta)
{
  UInt32 width  = window->size.x;
  UInt32 height = window->size.y;
  Float32 z;
  glm::vec3 result = glm::vec3(1.0);
  glReadPixels(width/2, height/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  result = glm::unProject(glm::vec3(width/2,height/2,z),
                          camera->view, camera->projection,
                          glm::vec4(0,0,width,height));

  if(delta != 0.0)
  {
    glm::vec3 viewVector = result - camera->position;
    viewVector = glm::normalize(viewVector);
    result += viewVector*delta;
  }
  return result;
}