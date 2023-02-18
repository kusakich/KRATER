#version 330 core

in  vec2 vTextureCoord;
out vec4 fColor;

uniform vec3      color;
uniform sampler2D uTexture0;

void main()
{
  fColor = vec4(color,texture(uTexture0, vTextureCoord).r);
}