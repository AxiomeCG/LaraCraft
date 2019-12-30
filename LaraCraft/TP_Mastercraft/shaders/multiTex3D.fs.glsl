#version 320 es
precision mediump float;

uniform sampler2D uTexture;
uniform sampler2D uTextureSecond;

in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;
in vec2 vFragTexture;

out vec3 fFragColor;


void main() {
    fFragColor = vec3(texture(uTexture, vFragTexture) + texture(uTextureSecond, vFragTexture));
    //fFragColor = normalize(vFragNormal_vs);
}
