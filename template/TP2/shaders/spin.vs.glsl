#version 330 core

#define M_PI 3.1415926535897932384626433832795

precision mediump float;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec3 vFragColor;
out vec2 vPos;

mat3 rotate(float a) {
  a *= M_PI / 180.;
  return mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0., 0., 1.));
}

uniform float uTime;
uniform mat3 uModelMatrix;

void main() {
  vPos = aVertexPosition;
  mat3 R = rotate(uTime);
  vFragColor = vec3(255., 0., 0.);
  gl_Position = vec4((uModelMatrix * vec3(aVertexPosition, 1)).xy, 0, 1);
}
