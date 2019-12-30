#version 320 es
precision mediump float;


layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

vec2 sum;
out vec3 vFragColor;

void main() {
    vFragColor = aVertexColor;
    vec2 sum = aVertexPosition * vec2(2,0.5);
    gl_Position = vec4(sum, 0, 1);
}