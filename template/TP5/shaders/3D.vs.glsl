#version 330 core

#define M_PI 3.1415926535897932384626433832795

precision mediump float;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexture;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 vPosCoord;
out vec3 vNormCoord;
out vec2 vTexCoord;

void main() {
  vec4 vPos = vec4(aVertexPosition, 1);
  vec4 vNorm = vec4(aVertexNormal, 0);

  vPosCoord = vec3(uMVMatrix * vPos);
  vNormCoord = vec3(uNormalMatrix * vNorm);

  vTexCoord = aVertexTexture;
  gl_Position = uMVPMatrix * vPos;
}
