#version 430 core

layout (location = 0) in vec4 vertex;	// Vec2 Position, vec2 TextureCoordinates //

out vec2 TexCoords;

uniform bool shake;
uniform bool invertColours;
uniform bool chaos;
uniform float time;

void main() {
	gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
	TexCoords = vertex.zw;

	// Shake the screen around.
	if (shake) {
		float strength = 0.0125f;
		gl_Position.x += cos(time * 10.0f) * strength;
		gl_Position.y += cos(time * 15.0f) * strength;
	}
}