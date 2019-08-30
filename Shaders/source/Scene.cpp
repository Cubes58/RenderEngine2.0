#include "Scene.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "Window.h"
#include "Camera.h"
#include "PostProcessor.h"
#include "Skybox.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "GameObject.h"

Scene::Scene(std::shared_ptr<Window> p_Window) : m_Window(p_Window) {
	m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));
	m_PostProcessor = std::make_shared<PostProcessor>((float)p_Window->Width(), (float)p_Window->Height());
	m_Skybox = std::make_shared<Skybox>();

	m_SceneObject = std::make_shared<GameObject>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "nanosuit", "blinnPhong");
	m_LightObject = std::make_shared<GameObject>(glm::vec3(10.5f, 15.5f, 15.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f), "sphere", "flat");
	m_LightObject->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
}

void Scene::HandleMouseInput(float p_XPosition, float p_YPosition) {
	static float s_PreviousXPosition = p_XPosition;
	static float s_PreviousYPosition = p_YPosition;

	m_Camera->ProcessMouseMovement(p_XPosition - s_PreviousXPosition, s_PreviousYPosition - p_YPosition);	// Reversed the Y-axis, since Y-coordinates range from bottom to top.

	s_PreviousXPosition = p_XPosition;
	s_PreviousYPosition = p_YPosition;
}

void Scene::HandleKeyboardInput(std::vector<bool> &p_KeyPressBuffer, std::vector<bool> &p_KeyReleaseBuffer) {
	// Camera movement:
	if (p_KeyPressBuffer['W'])
		m_Camera->ProcessKeyboard(CameraMovement::FORWARD, m_DeltaTime);
	if (p_KeyPressBuffer['S'])
		m_Camera->ProcessKeyboard(CameraMovement::BACKWARD, m_DeltaTime);
	if (p_KeyPressBuffer['A'])
		m_Camera->ProcessKeyboard(CameraMovement::LEFT, m_DeltaTime);
	if (p_KeyPressBuffer['D'])
		m_Camera->ProcessKeyboard(CameraMovement::RIGHT, m_DeltaTime);
	if (p_KeyPressBuffer['Q'])
		m_Camera->ProcessKeyboard(CameraMovement::UP, m_DeltaTime);
	if (p_KeyPressBuffer['E'])
		m_Camera->ProcessKeyboard(CameraMovement::DOWN, m_DeltaTime);
	if (p_KeyReleaseBuffer['B'])
		m_Camera->m_EnableSpeedBoost = !m_Camera->m_EnableSpeedBoost;
	if (p_KeyReleaseBuffer['1']) {
		if (m_PostProcessor->GetShakeState()) {
			m_PostProcessor->SetShakeState(false);
			std::cout << "\nShake effect: Off" << std::endl;
		}
		else {
			m_PostProcessor->SetShakeState(true);
			std::cout << "\nShake effect: On" << std::endl;
		}
	}
	if (p_KeyReleaseBuffer['2']) {
		if (m_PostProcessor->GetInvertColoursState()) {
			m_PostProcessor->SetInvertColoursState(false);
			std::cout << "\nInverted colour effect: Off" << std::endl;
		}
		else {
			m_PostProcessor->SetInvertColoursState(true);
			std::cout << "\nInverted colour effect: On" << std::endl;
		}
	}
	if (p_KeyReleaseBuffer['3']) {
		if (m_PostProcessor->GetChaosState()) {
			m_PostProcessor->SetChaosState(false);
			std::cout << "\nEdge kernel effect: Off" << std::endl;
		}
		else {
			m_PostProcessor->SetChaosState(true);
			std::cout << "\nEdge kernel effect: On" << std::endl;
		}
	}
	if (p_KeyReleaseBuffer['4']) {
		m_UseBlinnPhong = !m_UseBlinnPhong;
		if (m_UseBlinnPhong)
			std::cout << "\nBlinn phong: On" << std::endl;
		else 
			std::cout << "\nBlinn phong: Off" << std::endl;
	}
	if (p_KeyReleaseBuffer['5']) {
		m_UseNormalMap = !m_UseNormalMap;
		if (m_UseNormalMap)
			std::cout << "\nUsing normal maps: On" << std::endl;
		else
			std::cout << "\nUsing normal maps: Off" << std::endl;
	}
	if (p_KeyReleaseBuffer['6']) {
		m_UseToonShading = !m_UseToonShading;
		if (m_UseToonShading)
			std::cout << "\nToon shading: On" << std::endl;
		else
			std::cout << "\nToon shading: Off" << std::endl;
	}
	if (p_KeyReleaseBuffer['7']) {
		m_ShowNormalMap = !m_ShowNormalMap;
		if (m_ShowNormalMap)
			std::cout << "\nShow Normal Map: On" << std::endl;
		else
			std::cout << "\nShow Normal Map: Off" << std::endl;
	}

	for (auto releasedKey : p_KeyReleaseBuffer)
		releasedKey = false;
}

void Scene::Update(float p_DeltaTime) {
	m_DeltaTime = p_DeltaTime;

	m_PostProcessor->Update(p_DeltaTime);
}

void Scene::Render() {
	m_PostProcessor->BeginRender();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_Camera->m_Zoom), static_cast<float>(m_Window->Width()) / static_cast<float>(m_Window->Height()), m_NearClippingPlane, m_FarClippingPlane);
	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	for (auto &shader : ResourceManagerInstance.m_Shaders) {
		shader.second->Use();
		shader.second->SetMat4("projection", projectionMatrix);
		shader.second->SetMat4("view", viewMatrix);
		shader.second->SetMat4("viewWithoutTransform", glm::mat4(glm::mat3(viewMatrix)));

		shader.second->SetVec3("lightPosition", m_LightObject->GetPosition());
		shader.second->SetVec3("lightColour", m_LightObject->GetColour());
		shader.second->SetVec3("lightAttenuation", glm::vec3(1.0f, 0.022f, 0.0019f));
		shader.second->SetFloat("specularExponent", m_UseBlinnPhong ? 16.0f : 8.0f);
		shader.second->SetFloat("surfaceSpecularBrightness", 0.4f);
		shader.second->SetFloat("ambientStrength", 0.1f);

		shader.second->SetVec3("viewPosition", m_Camera->m_Position);
		shader.second->SetBool("blinn", m_UseBlinnPhong);
		shader.second->SetBool("useNormalMap", m_UseNormalMap);
		shader.second->SetBool("toonShading", m_UseToonShading);
		shader.second->SetBool("showNormalMap", m_ShowNormalMap);
	}
	m_LightObject->Render();
	m_SceneObject->Render();
	m_Skybox->Render();
	m_PostProcessor->Render();
}

bool Scene::IsRunning() const {
	return m_IsRunning;
}