#version 320 es
precision mediump float;


layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

vec2 sum;
out vec3 vFragColor;
out vec2 vFragPosition;
mat3 translate(float tx, float ty){
    mat3 M = mat3(
    vec3(1, 0, 0), // col1
    vec3(0, 1, 0), // col2
    vec3(tx, ty, 1)// col3
    );
    return M;
}

mat3 scale(float sx, float sy){
    mat3 M = mat3(
    vec3(sx, 0, 0), // col1
    vec3(0, sy, 0), // col2
    vec3(0, 0, 1)// col3
    );
    return M;
}

mat3 rotate(float a) {
    mat3 M = mat3(
    vec3(cos(radians(a)), sin(radians(a)), 0), // col1
    vec3(-sin(radians(a)), cos(radians(a)), 0), // col2
    vec3(0, 0, 1)// col3
    );
    return M;
}
void main() {
    vFragColor = aVertexColor;
    vFragPosition = aVertexPosition;
    mat3 T= translate(0.5,0.);
    mat3 R= rotate(90.);
    mat3 S= scale(0.5,0.5);
    gl_Position = vec4((T*R*vec3(aVertexPosition, 1)).xy, 0, 1);
}