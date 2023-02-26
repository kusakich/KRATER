#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include <util.hpp>

struct Block
{
  static const UInt32 MAX_LIGHT = 15;

  UInt8 type;
  UInt8 light;
};

class Chunk
{
public:
  static const UInt32 WIDTH  = 16;
  static const UInt32 VOLUME = WIDTH*WIDTH*WIDTH;
  Chunk(glm::uvec2 index);
  ~Chunk();

  void   setBlockType (UInt32 x, UInt32 y, UInt32 z, UInt32 type);
  UInt32 getBlockType (UInt32 x, UInt32 y, UInt32 z);
  UInt32 getBlockLight(UInt32 x, UInt32 y, UInt32 z);
  void   computeLight();

  glm::uvec2 index;
  Block blocks[WIDTH][WIDTH][WIDTH];
};

class World
{
public:
  static const UInt32 WIDTH = 4;

  World();
  ~World();

  void   setBlockType (UInt32 x, UInt32 y, UInt32 z, UInt32 type);
  UInt32 getBlockType (UInt32 x, UInt32 y, UInt32 z);
  UInt32 getBlockLight(UInt32 x, UInt32 y, UInt32 z);

  Chunk * getChunk(glm::uvec2 index);
private:
  Chunk * chunks[World::WIDTH][World::WIDTH];
};

#endif