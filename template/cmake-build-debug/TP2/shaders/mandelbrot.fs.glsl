#version 330 core

precision mediump float;

in vec3 vFragColor;
in vec2 vPos;

out vec3 fFragColor;

float N = 1000.;

vec2 complexSqr(vec2 z) {
	return vec2(z.x * z.x - z.y * z.y, 2. * z.x * z.y);
}

float mandelbrot(vec2 c) {
	vec2 z0 = c;
	vec2 z1;
	for(float i = 0.; i < N; i++) {
		z1 = complexSqr(z0) + c;
		if(length(z1) > 2.)
			return i;
		z0 = z1;
	}
	return -1.;
}

void main() {
	float j = mandelbrot(vPos);
	if(j < 0.) {
		fFragColor = vec3(1., 1., 1.);
	}
	else {
		fFragColor = vec3(j/25., 0, 0);
	}
}