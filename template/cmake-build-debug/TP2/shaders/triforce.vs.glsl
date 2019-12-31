#version 330 core

#define M_PI 3.1415926535897932384626433832795

precision mediump float;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec3 vFragColor;
out vec2 vPos;
out vec2 vTexCoord;

uniform mat3 uModelMatrix;

void main() {
  vPos = aVertexPosition;
  vTexCoord = aVertexTexture;
  gl_Position = vec4((uModelMatrix * vec3(aVertexPosition, 1)).xy, 0, 1);
}
