#version 330 core

precision mediump float;

in vec3 vFragColor;

out vec3 fFragColor;

void main() {
	fFragColor = vFragColor;
}