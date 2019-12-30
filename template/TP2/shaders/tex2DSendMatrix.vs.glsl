#version 320 es
precision mediump float;


uniform mat3 uModelMatrix;

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec2 aVertexTexture;

out vec3 vFragColor;

void main() {
    mat3 matrix = uModelMatrix;
    vFragColor = vec3(255,0,0);
    gl_Position = vec4((matrix*vec3(aVertexPosition,1)).xy,0,1);
}