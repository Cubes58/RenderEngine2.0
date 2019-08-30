#version 430 core

uniform vec3 surfaceColour;

out vec4 FragColour;

void main() {
	FragColour = vec4 (surfaceColour, 1.0f);
}