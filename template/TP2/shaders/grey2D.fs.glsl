#version 330 core

precision mediump float;

in vec3 vFragColor;

out vec3 fFragColor;

void main() {
  fFragColor = vec3((vFragColor.x + vFragColor.y + vFragColor.z) / 3.0, 
  	(vFragColor.x + vFragColor.y + vFragColor.z) / 3.0, 
  	(vFragColor.x + vFragColor.y + vFragColor.z) / 3.0);
}