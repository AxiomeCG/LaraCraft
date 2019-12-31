#version 330 core

precision mediump float;

in vec3 vFragColor;
in vec2 vPos;

out vec3 fFragColor;

float attenuation(vec2 P) {
	float dist = distance(P, vec2(0, 0));
	float a = 5.;
	float b = 100.;
	return a * exp(-b * dist * dist);
}

void main() {
	float a = attenuation(vPos);
	fFragColor = vFragColor * a;
}