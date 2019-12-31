#version 330 core

precision mediump float;

in vec3 vPosCoord;
in vec3 vNormCoord;
in vec2 vTexCoord;

out vec3 fFragColor;


uniform sampler2D uTexture;

void main() {
	fFragColor = normalize(vNormCoord);
}