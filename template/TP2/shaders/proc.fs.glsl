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

float grid1(vec2 P) {
	return length(fract(5.0 * P));
}

float grid2(vec2 P) {
	return length(abs(fract(5.0 * P) * 2.0 - 1.0));
}

float checkerboard(vec2 P) {
	return mod(floor(10.0 * P.x) + floor(10.0 * P.y), 2.0);
}

float holes(vec2 P) {
	return smoothstep(0.3, 0.5, length(fract(5.0 * P) - 0.5));
}

float brickwall(vec2 P) {
	return smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5)));
}

float procedure(vec2 P) {
	return log(length(10.0 * P.x) + length(fract(10. * P.y)));
}

void main() {
	float a = attenuation(vPos);
	fFragColor = vFragColor * a;
}