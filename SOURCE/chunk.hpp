#ifndef CHUNK_HPP_INCLUDED
#define CHUNK_HPP_INCLUDED

#include <util.hpp>
#include <mesh.hpp>

struct Chunk
{
  static const UInt32 WIDTH = 16;
  static const UInt32 VOLUME = WIDTH*WIDTH*WIDTH;

  union
  {
    UInt32 data[VOLUME];
    UInt32 data3[WIDTH][WIDTH][WIDTH];
  };

  Chunk();
};

#endif