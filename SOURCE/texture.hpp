#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

namespace gl
{
  enum Wrap
  {
    REPEAT = 0x2901,
    CLAMP_TO_EDGE = 0x812F
  };

  enum Filter
  {
    NEAREST = 0x2600,
    LINEAR = 0x2601
  };

  struct TextureSettings
  {
    Wrap   wrap;
    Filter filter;
  };

  class Texture
  {
  public:
    Texture(const char* fileName,
            TextureSettings settings =
            (TextureSettings){.wrap = REPEAT, .filter = NEAREST});
    ~Texture();
    void bind();
  private:
    UInt32 id;
  };
};

#endif