#version 330 core

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoord;

out vec3 vColor;
out vec2 vTextureCoord;

void main()
{
  vColor = vec3(1.0,1.0,1.0);
  vTextureCoord = aTextureCoord;
  gl_Position = p*v*m*vec4(aPosition, 1.0);
}