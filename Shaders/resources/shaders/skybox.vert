#version 430 core

layout (location = 0) in vec3 vertexPosition;

out vec3 TextureCoords;

uniform mat4 projection;
uniform mat4 viewWithoutTransform;

void main() {
	TextureCoords = vertexPosition;
	gl_Position = projection * viewWithoutTransform * vec4(vertexPosition, 1.0);
}