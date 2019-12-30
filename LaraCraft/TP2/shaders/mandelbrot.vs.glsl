#version 320 es
precision mediump float;


layout(location = 3) in vec2 aVertexPosition;

out vec3 vFragColor;
out vec2 vPos;

void main() {
  vPos = aVertexPosition;
  gl_Position = vec4(aVertexPosition, 0, 1);
}
