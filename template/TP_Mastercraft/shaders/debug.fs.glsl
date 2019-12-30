#version 320 es
precision mediump float;

//uniform sampler2D uTexture;

in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;
in vec2 vFragTexture;

out vec3 fFragColor;


void main() {
    //fFragColor = vec3(texture(uTexture, vFragTexture));
    fFragColor = normalize(vFragNormal_vs);
    //fFragColor = vec3(255,0,0);
}
