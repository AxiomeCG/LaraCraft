#version 320 es
precision mediump float;

in vec3 vFragColor;
in vec2 vFragPosition;
float m;
out vec3 fFragColor;

void main() {
    vec2 P = vFragPosition;
    fFragColor = vFragColor* smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5)));
}
