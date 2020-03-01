#version 330 core
precision mediump float;


uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexture;

out vec3 vFragPosition_vs;
out vec3 vFragNormal_vs;
out vec2 vFragTexture;


void main() {
    vFragPosition_vs = vec3(uMVMatrix * vec4(aVertexPosition, 1.));
    vFragNormal_vs = vec3(uNormalMatrix * vec4(aVertexNormal,0.));
    vFragTexture = aVertexTexture;

    gl_Position = uMVPMatrix*vec4(aVertexPosition,1);
}