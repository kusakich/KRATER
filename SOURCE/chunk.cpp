#include <chunk.hpp>

Chunk::Chunk()
{
  std::fill(data, &data[VOLUME-1], 0);
  for(UInt32 z = 1; z < WIDTH-1; z++)
    for(UInt32 x = 1; x < WIDTH-1; x++)
      for(UInt32 y = 1; y < WIDTH-1; y++)
      {
        Float64 id = glm::perlin(glm::vec3((Float64)x*0.2,(Float64)y*0.2,(Float64)z*0.2));
        if(id < -0.2) data3[z][y][x] = 0;
        else       data3[z][y][x] = 1;

        //if(z < 7) data3[z][y][x] = 1;
        //else      data3[z][y][x] = 0;
      }
}