#pragma once

#include <memory>
#include <vector>
#include <string>

class Window;
class Camera;
class PostProcessor;
class Skybox;
class GameObject;

class Scene {
private:
	std::shared_ptr<Window> m_Window;
	std::shared_ptr<Camera> m_Camera;
	std::shared_ptr<PostProcessor> m_PostProcessor;
	std::shared_ptr<Skybox> m_Skybox;

	std::shared_ptr<GameObject> m_SceneObject;
	std::shared_ptr<GameObject> m_LightObject;

	float m_FarClippingPlane = 100.0f;
	float m_NearClippingPlane = 0.1f;
	float m_DeltaTime = 0.0f;

	bool m_IsRunning = true;
	bool m_UseBlinnPhong = true;
	bool m_UseNormalMap = true;
	bool m_UseToonShading = true;
	bool m_ShowNormalMap = false;

public:
	Scene(std::shared_ptr<Window> p_Window);
	~Scene() = default;

	void HandleMouseInput(float p_XPosition, float p_YPosition);
	void HandleKeyboardInput(std::vector<bool> &p_KeyPressBuffer, std::vector<bool> &p_KeyReleaseBuffer);
	void Update(float p_DeltaTime);
	void Render();

	bool IsRunning() const;
};