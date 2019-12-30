#version 320 es
precision mediump float;

in vec3 vFragColor;
in vec2 vFragPosition;
float m;
out vec3 fFragColor;

void main() {
    float alpha = 5.;
    float beta = 50.;

    float d = distance(vFragPosition,vec2(0,0));
    float a = alpha * exp(-beta* d * d);
    fFragColor = vFragColor * a;
}
