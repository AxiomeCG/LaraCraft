#version 330 core

precision mediump float;

in vec3 vFragColor;
in vec2 vTexCoord;

out vec3 fFragColor;


uniform sampler2D uTexture;

void main() {
	fFragColor = vec3(texture(uTexture, vTexCoord));

}