#version 320 es
precision mediump float;

uniform sampler2D uTexture;

in vec2 vFragTexture;

out vec3 fFragColor;

void main() {
    fFragColor = vec3(texture(uTexture, vFragTexture));
}
