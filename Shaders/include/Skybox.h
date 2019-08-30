#pragma once

#include <memory>

class Shader;

class Skybox {
private:
	static const float m_SkyboxSize;
	static const unsigned int m_SkyboxNumberOfVertices = 108;
	float m_SkyboxVertices[m_SkyboxNumberOfVertices];

	std::shared_ptr<Shader> m_Shader;
	unsigned int m_CubeMapTextureID;
	unsigned int m_SkyBoxVBO;
	unsigned int m_SkyBoxVAO;

	void Prepare();

public:
	Skybox();

	void Render();
};