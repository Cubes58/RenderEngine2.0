#version 430 core

out vec4 FragSurfaceColour;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

uniform sampler2D textureDiffuse0;
uniform sampler2D textureNormal1;
uniform sampler2D textureSpecular2;

uniform vec3 lightPosition;
uniform vec3 lightAttenuation;
uniform vec3 viewPosition;

uniform bool blinn;
uniform bool useNormalMap;
uniform bool toonShading;
uniform bool showNormalMap;

uniform vec3 lightColour;
uniform float ambientStrength;
uniform float specularExponent;
uniform float surfaceSpecularBrightness;

// Toon shading.
const float levels = 4.0f;

void main() {
    vec3 surfaceColour = texture(textureDiffuse0, fs_in.TexCoords).rgb;

	// Light attenuation.
	float distance = length(lightPosition - fs_in.FragPos);
	float attenuationFactor = lightAttenuation.x / (lightAttenuation.x + lightAttenuation.y * distance + lightAttenuation.z * (distance * distance));

    // Ambient.
    vec3 ambient = lightColour * surfaceColour * ambientStrength;

	// Calculate ambient toon shading level.
	float ambientEffect = length(ambient);
	float level = floor(ambientEffect * levels);
	ambientEffect = level / levels;

	if (toonShading)
		ambient = ambient * ambientEffect;
	ambient *= attenuationFactor;

	// Normal mapping.
	vec3 normal = vec3(0.0f, 0.0f, 0.0f);
	if (useNormalMap) {
		normal = texture(textureNormal1, fs_in.TexCoords).rgb;
		// Transform the normal vector to the range: -1, 1.
		normal = normalize(normal * 2.0f - 1.0f);	
	}
	else {
		normal = normalize(fs_in.Normal);
	}

    // Diffuse.
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0f);	// Brightness.
	if (toonShading) {
		level = floor(diff * levels);
		diff = level / levels;
	}
    vec3 diffuse = (lightColour * diff * surfaceColour) * attenuationFactor;

    // Specular.
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    float spec = 0.0;
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0f), specularExponent);
    }
    else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0f), specularExponent);
    }

	if(toonShading) {
		level = floor(spec * levels);
		spec = level / levels;
	}
    vec3 specular = (lightColour * surfaceSpecularBrightness * spec) * attenuationFactor;
	
    FragSurfaceColour = vec4(ambient + diffuse + specular, 1.0f);
	if(showNormalMap)
		FragSurfaceColour = vec4(vec3(texture(textureNormal1, fs_in.TexCoords).rgb), 1.0f);
}