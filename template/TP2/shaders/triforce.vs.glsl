#version 320 es
precision mediump float;


uniform mat3 uModelMatrix;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vFragTexture;

void main() {
    vFragTexture = aVertexTexture;
    mat3 matrix = uModelMatrix;
    gl_Position = vec4((matrix*vec3(aVertexPosition,1)).xy,0,1);
}