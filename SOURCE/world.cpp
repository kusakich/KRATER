#include <world.hpp>

Chunk::Chunk(UInt32 globalX, UInt32 globalY)
{
  for(UInt32 z = 0; z < Chunk::WIDTH; z++)
    for(UInt32 x = 0; x < Chunk::WIDTH; x++)
      for(UInt32 y = 0; y < Chunk::WIDTH; y++)
      {
        data.blocks[z][y][x].type  = 0;
        data.blocks[z][y][x].light = 0;
      }

  for(UInt32 z = 0; z < Chunk::WIDTH-8; z++)
    for(UInt32 x = 0; x < Chunk::WIDTH; x++)
      for(UInt32 y = 0; y < Chunk::WIDTH; y++)
      {
        Float64 id = glm::perlin(glm::vec3((Float64)(x+globalX)*0.1,(Float64)(y+globalY)*0.1,(Float64)z*0.1));
        if(id < -0.1) data.blocks[z][y][x].type = 0;
        else          data.blocks[z][y][x].type = 1;

        if(z == 0) data.blocks[z][y][x].type = 1;
      }
}

ChunkData Chunk::getData()
{
  return data;
}

void Chunk::setBlockType(UInt32 x, UInt32 y, UInt32 z, UInt32 type)
{
  if( x >= 0 && x < Chunk::WIDTH &&
      y >= 0 && y < Chunk::WIDTH &&
      z >= 0 && z < Chunk::WIDTH) data.blocks[z][y][x].type = type;
}

void Chunk::computeLight()
{
  for(UInt32 z = 0; z < Chunk::WIDTH; z++)
    for(UInt32 x = 0; x < Chunk::WIDTH; x++)
      for(UInt32 y = 0; y < Chunk::WIDTH; y++)
      {
        data.blocks[z][y][x].light = 0;
      }
  for(UInt32 x = 0; x < Chunk::WIDTH; x++)
    for(UInt32 y = 0; y < Chunk::WIDTH; y++)
    {
      for(SInt32 z = Chunk::WIDTH-1; z >= 0; z--)
      {
        data.blocks[z][y][x].light = Block::MAX_LIGHT;
        if (data.blocks[z][y][x].type) break;
      }
    }
}

World::World()
{
  for(UInt32 i = 0; i < World::WIDTH; ++i)
    for (UInt32 j = 0; j < World::WIDTH; ++j)
    {
      chunks[i][j] = nullptr;
      chunks[i][j] = new Chunk(j*Chunk::WIDTH, i*Chunk::WIDTH);
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