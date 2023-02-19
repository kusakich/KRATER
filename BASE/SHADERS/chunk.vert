#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in float aLight;

out vec3 vPosition;
out vec3 vNormal;
out float vLight;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
  vPosition = aPosition;
  vNormal = normalize(aNormal);
  vLight = aLight;

  gl_Position = p*v*m*vec4(aPosition, 1.0);
}