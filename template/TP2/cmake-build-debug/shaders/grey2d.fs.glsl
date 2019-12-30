#version 320 es
precision mediump float;

in vec3 vFragColor;
float m;
out vec3 fFragColor;

void main() {
    m = (vFragColor.r + vFragColor.g + vFragColor.b)/3.0;
    fFragColor = vec3(m,m,m);
}
