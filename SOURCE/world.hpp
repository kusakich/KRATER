#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include <util.hpp>

struct Block
{
  static const UInt32 MAX_LIGHT = 15;

  UInt8 type;
  UInt8 light;
};

struct ChunkData
{
  static const UInt32 WIDTH  = 16;
  static const UInt32 VOLUME = WIDTH*WIDTH*WIDTH;

  Block blocks[ChunkData::WIDTH][ChunkData::WIDTH][ChunkData::WIDTH];
};

class Chunk
{
public:
  static const UInt32 WIDTH  = 16;
  static const UInt32 VOLUME = WIDTH*WIDTH*WIDTH;
  Chunk(UInt32 globalX = 0, UInt32 globalY = 0);
  ~Chunk();
  void setBlockType(UInt32 x, UInt32 y, UInt32 z, UInt32 type);
  ChunkData getData();
  void computeLight();
private:
  ChunkData data;
};

class World
{
public:
  static const UInt32 WIDTH = 4;

  World();
  ~World();

  void setBlockType(UInt32 x, UInt32 y, UInt32 z, UInt32 type);
  Chunk * getChunk(UInt32 i, UInt32 j);
private:
  Chunk * chunks[World::WIDTH][World::WIDTH];
};

#endif