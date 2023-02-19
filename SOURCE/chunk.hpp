#ifndef CHUNK_HPP_INCLUDED
#define CHUNK_HPP_INCLUDED

#include <util.hpp>
#include <mesh.hpp>

struct Block
{
  static const UInt32 MAX_LIGHT = 15;

  UInt8 type;
  UInt8 light;
};

struct Chunk
{
  static const UInt32 WIDTH = 16;
  static const UInt32 VOLUME = WIDTH*WIDTH*WIDTH;

  union
  {
    Block data[VOLUME];
    Block data3[WIDTH][WIDTH][WIDTH];
  };

  Chunk();
  void computeLight();
};

#endif