#version 330 core

precision mediump float;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;
out vec2 vPos;

void main() {
  vFragColor = aVertexColor;
  vPos = aVertexPosition;
  gl_Position = vec4(aVertexPosition, 0, 1);
}
