#pragma once

#include <memory>
#include <string>

#include <glm/vec3.hpp>

class Model;
class Shader;

class GameObject {
private:
	glm::vec3 m_Position;
	glm::vec3 m_Orientation;
	glm::vec3 m_Scale;
	glm::vec3 m_Colour;
	std::shared_ptr<Model> m_Model;
	std::shared_ptr<Shader> m_Shader;
	
public:
	GameObject();
	GameObject(const glm::vec3 &p_Position, const glm::vec3 &p_Orientation, const glm::vec3 &p_Scale,
		const std::string &p_ModelName, const std::string &p_ShaderName, const glm::vec3 &p_Colour = glm::vec3(1.0f, 1.0f, 1.0f));
	~GameObject() = default;


	void Update(float p_DeltaTime);
	void Render();

	inline void SetPosition(const glm::vec3 &p_Position) {
		m_Position = p_Position;
	}
	inline glm::vec3 GetPosition() {
		return m_Position;
	}

	inline void SetOrientation(const glm::vec3 &p_Orientation) {
		m_Orientation = p_Orientation;
	}
	inline glm::vec3 GetOrientation() {
		return m_Orientation;
	}

	inline void SetColour(const glm::vec3 &p_Colour) {
		m_Colour = p_Colour;
	}
	inline glm::vec3 GetColour() {
		return m_Colour;
	}
};