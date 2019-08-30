#version 430 core

in vec3 TextureCoords;

out vec4 FragColour;

uniform samplerCube skybox;

void main() {
	FragColour = texture(skybox, TextureCoords);
}