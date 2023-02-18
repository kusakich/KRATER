#version 330 core

uniform mat4 m;
uniform mat4 v;

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTextureCoord;

out vec3 vColor;
out vec2 vTextureCoord;

void main()
{
  vColor = vec3(1.0);
  vTextureCoord = aTextureCoord;
  gl_Position = v*m*vec4(aPosition, 1.0, 1.0);
}