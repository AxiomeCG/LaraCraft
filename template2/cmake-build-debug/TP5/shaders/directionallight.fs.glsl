#version 330 core

precision mediump float;

in vec3 vPosCoord;
in vec3 vNormCoord;
in vec2 vTexCoord;

out vec3 fFragColor;


uniform sampler2D uTexture;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong() {
	return vec3(uLightIntensity * (uKd * (dot(normalize(uLightDir_vs), vNormCoord)) + uKs * pow((dot(normalize(-vPosCoord), vNormCoord)), uShininess)));
}

void main() {
	fFragColor = blinnPhong();
}