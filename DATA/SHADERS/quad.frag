#version 330 core

in  vec3 vColor;
in  vec2 vTextureCoord;

out vec4 fColor;

uniform sampler2D uTexture0;

void main()
{
  fColor = vec4(vColor,1.0)*texture(uTexture0, vTextureCoord);
}