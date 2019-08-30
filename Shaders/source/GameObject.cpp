#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"
#include "Model.h"
#include "Shader.h"

GameObject::GameObject() : m_Position(0.0f, 0.0f, 0.0f), m_Orientation(0.0f, 0.0f, 0.0f), 
	m_Scale(1.0f, 1.0f, 1.0f), m_Colour(glm::vec3(1.0f, 1.0f, 1.0f)) {
	m_Model = ResourceManagerInstance.GetModel("default");
	m_Shader = ResourceManagerInstance.GetShader("default");
}

GameObject::GameObject(const glm::vec3 &p_Position, const glm::vec3 &p_Orientation, const glm::vec3 &p_Scale, 
	const std::string &p_ModelName, const std::string &p_ShaderName, const glm::vec3 &p_Colour)
	: m_Position(p_Position), m_Orientation(p_Orientation), m_Scale(p_Scale), m_Colour(p_Colour) {
	m_Model = ResourceManagerInstance.GetModel(p_ModelName);
	m_Shader = ResourceManagerInstance.GetShader(p_ShaderName);
}

void GameObject::Update(float p_DeltaTime) {
	
}

void GameObject::Render() {
	m_Shader->Use();

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	// Translate:
	modelMatrix = glm::translate(modelMatrix, m_Position);
	// Rotate:
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Orientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Orientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// Scale:
	modelMatrix = glm::scale(modelMatrix, m_Scale);
	m_Shader->SetMat4("model", modelMatrix);
	m_Shader->SetVec3("surfaceColour", m_Colour);
	
	m_Model->Render(m_Shader->GetID());
}