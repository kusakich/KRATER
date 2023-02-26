#include <world.hpp>

Chunk::Chunk(glm::uvec2 pindex) : index(pindex)
{
  for(UInt32 z = 0; z < Chunk::WIDTH; z++)
    for(UInt32 x = 0; x < Chunk::WIDTH; x++)
      for(UInt32 y = 0; y < Chunk::WIDTH; y++)
      {
        blocks[z][y][x].type  = 0;
        blocks[z][y][x].light = 0;
      }

  for(UInt32 x = 0; x < Chunk::WIDTH; x++)
    for(UInt32 y = 0; y < Chunk::WIDTH; y++)
    {
      Float64 h =
        glm::perlin(glm::vec3(
                    (Float64)(x+index.x*WIDTH)*0.1,
                    (Float64)(y+index.y*WIDTH)*0.1,
                    0.0));

      for(UInt32 z = 0; z < Chunk::WIDTH; z++)
      {
        if(z < (h+0.5)*7+2)
          blocks[z][y][x].type = 1;
      }
    }
}

void Chunk::setBlockType(UInt32 x, UInt32 y, UInt32 z, UInt32 type)
{
  if( x >= 0 && x < Chunk::WIDTH &&
      y >= 0 && y < Chunk::WIDTH &&
      z >= 0 && z < Chunk::WIDTH )
    blocks[z][y][x].type = type;
}

UInt32 Chunk::getBlockType (UInt32 x, UInt32 y, UInt32 z)
{
  if( x >= 0 && x < Chunk::WIDTH &&
      y >= 0 && y < Chunk::WIDTH &&
      z >= 0 && z < Chunk::WIDTH )
    return blocks[z][y][x].type;

  return 0;
}

UInt32 Chunk::getBlockLight (UInt32 x, UInt32 y, UInt32 z)
{
  if( x >= 0 && x < Chunk::WIDTH &&
      y >= 0 && y < Chunk::WIDTH &&
      z >= 0 && z < Chunk::WIDTH )
    return blocks[z][y][x].light;

  return 0;
}

void Chunk::computeLight()
{
  for(UInt32 z = 0; z < Chunk::WIDTH; z++)
    for(UInt32 x = 0; x < Chunk::WIDTH; x++)
      for(UInt32 y = 0; y < Chunk::WIDTH; y++)
      {
        blocks[z][y][x].light = 0;
      }
  for(UInt32 x = 0; x < Chunk::WIDTH; x++)
    for(UInt32 y = 0; y < Chunk::WIDTH; y++)
    {
      for(SInt32 z = Chunk::WIDTH-1; z >= 0; z--)
      {
        blocks[z][y][x].light = Block::MAX_LIGHT;
        if (blocks[z][y][x].type) break;
      }
    }
}

World::World()
{
  for(UInt32 i = 0; i < World::WIDTH; ++i)
    for(UInt32 j = 0; j < World::WIDTH; ++j)
    {
      chunks[i][j] = nullptr;
      chunks[i][j] = new Chunk(glm::uvec2(j, i));
      chunks[i][j]->computeLight();
    }
}

World::~World()
{
  for(UInt32 y = 0; y < World::WIDTH; ++y)
    for(UInt32 x = 0; x < World::WIDTH; ++x)
    {
      //if(chunks[y][x] != nullptr)
      //  delete chunks[y][x];
    }
}

void World::setBlockType(UInt32 x, UInt32 y, UInt32 z, UInt32 type)
{
  if( x >= 0 && x < Chunk::WIDTH*World::WIDTH &&
      y >= 0 && y < Chunk::WIDTH*World::WIDTH &&
      z >= 0 && z < Chunk::WIDTH*World::WIDTH ) {
    if(chunks[y/Chunk::WIDTH][x/Chunk::WIDTH] != nullptr) {
      chunks[y/Chunk::WIDTH][x/Chunk::WIDTH]->
        setBlockType(x%Chunk::WIDTH,
                     y%Chunk::WIDTH,
                     z%Chunk::WIDTH,
                     type);
    }
  }
}

UInt32 World::getBlockType(UInt32 x, UInt32 y, UInt32 z)
{
  if( x >= 0 && x < Chunk::WIDTH*World::WIDTH &&
      y >= 0 && y < Chunk::WIDTH*World::WIDTH &&
      z >= 0 && z < Chunk::WIDTH*World::WIDTH ) {
    if(chunks[y/Chunk::WIDTH][x/Chunk::WIDTH] != nullptr) {
      return chunks[y/Chunk::WIDTH][x/Chunk::WIDTH]->
          getBlockType(
          x%Chunk::WIDTH,
          y%Chunk::WIDTH,
          z%Chunk::WIDTH);
    }
  }
  return 0;
}

UInt32 World::getBlockLight(UInt32 x, UInt32 y, UInt32 z)
{
  if( x >= 0 && x < Chunk::WIDTH*World::WIDTH &&
      y >= 0 && y < Chunk::WIDTH*World::WIDTH &&
      z >= 0 && z < Chunk::WIDTH*World::WIDTH ) {
    if(chunks[y/Chunk::WIDTH][x/Chunk::WIDTH] != nullptr)
    {
      return chunks[y/Chunk::WIDTH][x/Chunk::WIDTH]->
        getBlockLight(
          x%Chunk::WIDTH,
          y%Chunk::WIDTH,
          z%Chunk::WIDTH);
    }
  }
  return 0;
}

Chunk * World::getChunk(glm::uvec2 index)
{
  if(index.x >= 0 && index.x < World::WIDTH &&
     index.y >= 0 && index.y < World::WIDTH) return chunks[index.y][index.x];

  return nullptr;
}