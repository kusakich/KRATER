#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <util.hpp>
#include <core.hpp>
#include <camera.hpp>
#include <primitives.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <chunk.hpp>
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

namespace gl
{
  namespace renderer
  {
    static Mesh*          basicRectMesh;
    static ShaderProgram* basicRectShaderProgram;

    static Mesh*          basicQuadMesh;
    static ShaderProgram* basicQuadShaderProgram;

    static ShaderProgram* basicTextShaderProgram;

    struct Character
    {
      UInt32     texture;
      glm::ivec2 size;
      glm::ivec2 bearing;
      SInt64     advance;
    };
    static std::map<char, Character> characters;

    void initialize();
    void finalize();
    void drawQuad(Quad quad, Camera camera);
    void drawRect(Rect rect);
    void drawText(std::string text, Float32 x, Float32 y, Float32 scale);

    template<typename T> Mesh* renderObject(T object);
  }
}

void gl::renderer::initialize()
{
  ///////////RECT///////////
  basicRectMesh = new Mesh(basicRectVertex, 6, (UInt32[]){2,2,0}, GL_TRIANGLES);
  Shader* basicRectShaders[2];
  basicRectShaders[0] = new Shader("BASE/SHADERS/rect.vert", gl::Shader::Type::VERTEX);
  basicRectShaders[1] = new Shader("BASE/SHADERS/rect.frag", gl::Shader::Type::FRAGMENT);
  basicRectShaderProgram = new ShaderProgram(2, basicRectShaders);
  basicRectShaderProgram->addUniform("m");
  basicRectShaderProgram->addUniform("v");

  ///////////QUAD///////////
  basicQuadMesh = new Mesh(basicQuadVertex, 36, (UInt32[]){3,3,2,0}, GL_TRIANGLES);
  Shader* basicQuadShaders[2];
  basicQuadShaders[0] = new Shader("BASE/SHADERS/quad.vert", gl::Shader::Type::VERTEX);
  basicQuadShaders[1] = new Shader("BASE/SHADERS/quad.frag", gl::Shader::Type::FRAGMENT);
  basicQuadShaderProgram = new ShaderProgram(2, basicQuadShaders);
  basicQuadShaderProgram->addUniform("m");
  basicQuadShaderProgram->addUniform("v");
  basicQuadShaderProgram->addUniform("p");

  ///////////TEXT///////////
  Shader* basicTextShaders[2];
  basicTextShaders[0] = new Shader("BASE/SHADERS/text.vert", gl::Shader::Type::VERTEX);
  basicTextShaders[1] = new Shader("BASE/SHADERS/text.frag", gl::Shader::Type::FRAGMENT);
  basicTextShaderProgram = new ShaderProgram(2, basicTextShaders);
  basicTextShaderProgram->addUniform("m");
  basicTextShaderProgram->addUniform("v");
  basicTextShaderProgram->addUniform("color");

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    spdlog::critical("ERROR::FREETYPE: Could not init FreeType Library");
    return; }
  FT_Face face;
  if (FT_New_Face(ft, "BASE/GUI/arial.ttf", 0, &face)) {
    spdlog::critical("ERROR::FREETYPE: Failed to load font");
    return; }

  FT_Set_Pixel_Sizes(face, 0, 48);
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

void gl::renderer::finalize()
{
  delete basicRectMesh;
  delete basicRectShaderProgram;
  delete basicQuadMesh;
  delete basicQuadShaderProgram;
  delete basicTextShaderProgram;
}

void gl::renderer::drawRect(Rect rect)
{
  basicRectShaderProgram->bind();
  glm::mat4 rectModel =
  { rect.size.x, 0.0, 0.0, 0.0,
    0.0, rect.size.y, 0.0, 0.0,
    0.0,         0.0, 1.0, 0.0,
    rect.position.x,
    rect.position.y,  0.0, 1.0};
  glm::mat4 rectView  = glm::ortho(0.0f, (Float32)core::window::getWidth(), (Float32)core::window::getHeight(), 0.0f);
  basicRectShaderProgram->uniform<glm::mat4>("m", rectModel);
  basicRectShaderProgram->uniform<glm::mat4>("v", rectView);
  basicRectMesh->draw();
}

void gl::renderer::drawQuad(Quad quad, Camera camera)
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

void gl::renderer::drawText(std::string text, Float32 x, Float32 y, Float32 scale)
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
    glm::mat4 rectView  = glm::ortho(0.0f, (Float32)core::window::getWidth(), (Float32)core::window::getHeight(), 0.0f);
    basicTextShaderProgram->uniform<glm::mat4>("m", rectModel);
    basicTextShaderProgram->uniform<glm::mat4>("v", rectView);
    basicTextShaderProgram->uniform<glm::vec3>("color", glm::vec3(1.0));
    basicRectMesh->draw();
    x += (character.advance >> 6)*scale;
  }
}

template<>
gl::Mesh* gl::renderer::renderObject<Chunk>(Chunk chunk)
{
  const   UInt32  vertexSize = 7;
  static  Float32 vertexBuffer[300000] = {0};
  std::fill(vertexBuffer, &vertexBuffer[299999], 0);
  UInt32  vertexCount = 0;
  for(UInt32 z = 0; z < Chunk::WIDTH-1; z++) {
    for(UInt32 y = 0; y < Chunk::WIDTH-1; y++) {
      for(UInt32 x = 0; x < Chunk::WIDTH-1; x++) {
        UInt8 index = 0;
        index = index | ((1 << 0) & ((!!chunk.data3[ z ][y+1][ x ].type) << 0));
        index = index | ((1 << 1) & ((!!chunk.data3[ z ][y+1][x+1].type) << 1));
        index = index | ((1 << 2) & ((!!chunk.data3[ z ][ y ][x+1].type) << 2));
        index = index | ((1 << 3) & ((!!chunk.data3[ z ][ y ][ x ].type) << 3));
        index = index | ((1 << 4) & ((!!chunk.data3[z+1][y+1][ x ].type) << 4));
        index = index | ((1 << 5) & ((!!chunk.data3[z+1][y+1][x+1].type) << 5));
        index = index | ((1 << 6) & ((!!chunk.data3[z+1][ y ][x+1].type) << 6));
        index = index | ((1 << 7) & ((!!chunk.data3[z+1][ y ][ x ].type) << 7));

        if((!index) || (index==255))
          continue;

        for(UInt32 i = 0; i < 16; i++)
        {
          if(trianglesTable[index][i] < 0 || trianglesTable[index][i] > 11)
            break;

          vertexBuffer[vertexCount*vertexSize+0] = (Float32)x + pointsTable[trianglesTable[index][i]][0];
          vertexBuffer[vertexCount*vertexSize+1] = (Float32)y + pointsTable[trianglesTable[index][i]][1];
          vertexBuffer[vertexCount*vertexSize+2] = (Float32)z + pointsTable[trianglesTable[index][i]][2];
          Float32 light = (Float32)
                          ((chunk.data3[ z ][y+1][ x ].light+
                            chunk.data3[ z ][y+1][x+1].light+
                            chunk.data3[ z ][ y ][x+1].light+
                            chunk.data3[ z ][ y ][ x ].light+
                            chunk.data3[z+1][y+1][ x ].light+
                            chunk.data3[z+1][y+1][x+1].light+
                            chunk.data3[z+1][ y ][x+1].light+
                            chunk.data3[z+1][ y ][ x ].light)/8)/
                            (Float32)Block::MAX_LIGHT;

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