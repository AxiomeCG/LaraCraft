#version 320 es
precision mediump float;

uniform sampler2D uTexture;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;


in vec3 vFragPosition_vs;
in vec3 vFragNormal_vs;
in vec2 vFragTexture;

out vec3 fFragColor;

vec3 blinnPhong() {
    vec3 hv = (normalize(-vFragPosition_vs) +  normalize(uLightDir_vs)) / 2.;
    float dotHvNormal = dot(hv, vec3(normalize(vFragNormal_vs)));
    float dot = dot(normalize(uLightDir_vs), normalize(vFragNormal_vs));
    if(dot < 0.) {
        dot = 0.;
    }

    return vec3(uLightIntensity * (uKd * dot) + uKs * dotHvNormal);
}

void main() {
    fFragColor = vec3(texture(uTexture,vFragTexture));//* blinnPhong();
}
