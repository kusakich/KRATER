#include <glad/glad.h>
#define  STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <util.hpp>
#include <texture.hpp>

gl::Texture::Texture(const char* fileName, const TextureSettings settings)
{
    SInt32 width, height, count;
    UInt32 target;

    UInt8* data = stbi_load(fileName, &width, &height, &count, 0);

    if(!data)
      spdlog::critical("File not found {0}", fileName);
    //if (proc)
    //    proc(data, width, height, count);

    glPixelStorei(GL_UNPACK_ALIGNMENT, count);
    glGenTextures(1, &target);
    glBindTexture(GL_TEXTURE_2D, target);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     settings.wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     settings.wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filter);
        glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                                       0, count == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    id = target;
}

gl::Texture::~Texture()
{
  glDeleteTextures(1, &id);
}

void gl::Texture::bind()
{
  glBindTexture(GL_TEXTURE_2D, id);
}