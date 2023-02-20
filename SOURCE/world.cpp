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

  for(UInt32 z = 0; z < Chunk::WIDTH-10; z++)
    for(UInt32 x = 0; x < Chunk::WIDTH; x++)
      for(UInt32 y = 0; y < Chunk::WIDTH; y++)
      {
        Float64 id = glm::perlin(glm::vec3((Float64)(x+globalX)*0.1,(Float64)(y+globalY)*0.1,(Float64)z*0.1));
        if(id < -0.1) data.blocks[z][y][x].type = 0;
        else          data.blocks[z][y][x].type = 1;

        //if(z < 7) data3[z][y][x] = 1;
        //else      data3[z][y][x] = 0;
      }
}

ChunkData Chunk::getData()
{
  return data;
}

void Chunk::computeLight()
{
  for(UInt32 x = 0; x < Chunk::WIDTH; x++)
    for(UInt32 y = 0; y < Chunk::WIDTH; y++)
    {
      for(UInt32 z = Chunk::WIDTH-1; z != 0; z--)
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

Chunk * World::getChunk(UInt32 x, UInt32 y)
{
  return chunks[y][x];
}