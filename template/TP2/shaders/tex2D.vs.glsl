#version 330 core

precision mediump float;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec3 vFragColor;
out vec2 vPos;

void main() {
  vPos = aVertexPosition;
  vFragColor = vec3(255., 0., 0.);
  gl_Position = vec4(aVertexPosition, 0, 1);
}
