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
      Float64 h = glm::perlin(glm::vec3((Float64)(x+index.x*WIDTH)*0.1,
                                        (Float64)(y+index.y*WIDTH)*0.1,
                                        0.0));

      for(UInt32 z = 0; z < Chunk::WIDTH; z++)
      {
        if(z < (h+0.5)*10)
          blocks[z][y][x].type = 1;
      }
    }
}

void Chunk::setBlockType(UInt32 x, UInt32 y, UInt32 z, UInt32 type)
{
  if( x >= 0 && x < Chunk::WIDTH &&
      y >= 0 && y < Chunk::WIDTH &&
      z >= 0 && z < Chunk::WIDTH) blocks[z][y][x].type = type;
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
    for (UInt32 j = 0; j < World::WIDTH; ++j)
    {
      chunks[i][j] = nullptr;
      chunks[i][j] = new Chunk(glm::uvec2(j, i));
      chunks[i][j]->computeLight();
    }
}

World::~World()
{
  for(UInt32 y = 0; y < World::WIDTH; ++y)
    for (UInt32 x = 0; x < World::WIDTH; ++x)
    {
      //if(chunks[y][x] != nullptr)
      //  delete chunks[y][x];
    }
}

void World::setBlockType(UInt32 x, UInt32 y, UInt32 z, UInt32 type)
{
  if( x >= 0 && x < Chunk::WIDTH*World::WIDTH &&
      y >= 0 && y < Chunk::WIDTH*World::WIDTH &&
      z >= 0 && z < Chunk::WIDTH*World::WIDTH)
  {
    if(chunks[y/Chunk::WIDTH][x/Chunk::WIDTH] != nullptr)
    {
      chunks[y/Chunk::WIDTH][x/Chunk::WIDTH]->setBlockType(x%Chunk::WIDTH,y%Chunk::WIDTH,z%Chunk::WIDTH, type);
    }
  }
}

Chunk * World::getChunk(UInt32 i, UInt32 j)
{
  if(i >= 0 && i < World::WIDTH &&
     j >= 0 && j < World::WIDTH) return chunks[j][i];

  return nullptr;
}