#version 430 core

in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edgeKernel[9];
uniform float blurKernel[9];
uniform bool shake;
uniform bool invertColours;
uniform bool chaos;

void main() {
	FragColour = vec4(0.0f);
	vec3 sampleData[9];

	// Sample from texture offsets if using convolution matrix.
	if (chaos || shake) {
		for (int i = 0; i < 9; i++) {
			sampleData[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
		}
	}

	// Process effects.
	if (invertColours) {
		FragColour = vec4(1.0f - texture(scene, TexCoords).rgb, 1.0f);
	}
	else if (chaos) {
		for (int i = 0; i < 9; i++) {
			FragColour += vec4(sampleData[i] * edgeKernel[i], 0.0f);
		}
		FragColour.a = 1.0f;
	}
	else if (shake) {
		for (int i = 0; i < 9; i++) {
			FragColour += vec4(sampleData[i] * blurKernel[i], 0.0f);
		}
		FragColour.a = 1.0f;
	}
	else {
		FragColour = texture(scene, TexCoords);
	}
}