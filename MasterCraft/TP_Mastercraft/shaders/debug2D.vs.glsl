#version 330 core
precision mediump float;


layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexture;

out vec3 vFragPosition_vs;
out vec3 vFragNormal_vs;
out vec2 vFragTexture;


void main() {
    vFragPosition_vs = aVertexPosition;
    vFragNormal_vs = aVertexNormal;
    vFragTexture = aVertexTexture;

    gl_Position = vec4(aVertexPosition,1);
}