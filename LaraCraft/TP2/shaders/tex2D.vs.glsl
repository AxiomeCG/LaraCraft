#version 320 es
precision mediump float;


uniform float uTime;

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec2 aVertexTexture;

out vec3 vFragColor;

mat3 rotate(float a) {
    mat3 M = mat3(
    vec3(cos(radians(a)), sin(radians(a)), 0), // col1
    vec3(-sin(radians(a)), cos(radians(a)), 0), // col2
    vec3(0, 0, 1)// col3
    );
    return M;
}

void main() {
    mat3 R = rotate(uTime);
    vFragColor = vec3(255,0,0);
    gl_Position = vec4((R*vec3(aVertexPosition,1)).xy,0,1);
}