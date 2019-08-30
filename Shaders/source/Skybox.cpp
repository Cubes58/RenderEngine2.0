#include "Skybox.h"

#include <vector>

#include "GLAD/glad.h"

#include "ResourceManager.h"
#include "Shader.h"

const float Skybox::m_SkyboxSize(55.0f);

Skybox::Skybox() : m_SkyboxVertices{
	// Vertices' positions.
	-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
	-m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
	-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,

	-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
	-m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
	-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
	-m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,

	m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,

	-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
	-m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
	-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,

	-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	-m_SkyboxSize,  m_SkyboxSize,  m_SkyboxSize,
	-m_SkyboxSize,  m_SkyboxSize, -m_SkyboxSize,

	-m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize, -m_SkyboxSize,
	-m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize,
	m_SkyboxSize, -m_SkyboxSize,  m_SkyboxSize } {
	m_Shader = ResourceManagerInstance.GetShader("skybox");
	Prepare();
}

void Skybox::Prepare() {
	glGenVertexArrays(1, &m_SkyBoxVAO);
	glGenBuffers(1, &m_SkyBoxVBO);
	glBindVertexArray(m_SkyBoxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_SkyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, Skybox::m_SkyboxNumberOfVertices * sizeof(GLfloat), &m_SkyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

	// Add the faces to a vector, so they can be passed and processed.
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("resources/textures/skyBox/right.bmp");
	skyboxFaces.push_back("resources/textures/skyBox/left.bmp");
	skyboxFaces.push_back("resources/textures/skyBox/top.bmp");
	skyboxFaces.push_back("resources/textures/skyBox/bottom.bmp");
	skyboxFaces.push_back("resources/textures/skyBox/front.bmp");
	skyboxFaces.push_back("resources/textures/skyBox/back.bmp");

	// Process the faces - creating the cubemap.
	m_CubeMapTextureID = ResourceManager::LoadOpenGLCubemapTexture(skyboxFaces);
}

void Skybox::Render() {
	glDepthFunc(GL_LEQUAL);		// The incoming depth value is less than or equal to the stored depth value.
	m_Shader->Use();

	glBindVertexArray(m_SkyBoxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}