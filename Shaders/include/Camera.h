#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement : unsigned int {
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,

	NOT_AVAILABLE
};

// Default camera values.
static const float s_Yaw = -90.0f;
static const float s_Pitch = 0.0f;
static const float s_Speed = 10.0f;
static const float s_Sensitivity = 0.15f;
static const float s_Zoom = 45.0f;
static const float  s_MaxPitchAngle = 89.0f;

class Camera {
private:
	void UpdateCameraVectors();
	void NormalizeAngle();

public:
	// Camera Attributes:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	// Euler Angles:
	float m_Yaw;
	float m_Pitch;

	// Camera options.
	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;
	float m_BoostSpeedMultiplayer = 1.75f;
	bool m_EnableSpeedBoost = false;

	Camera(const glm::vec3 &p_Position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &p_Up = glm::vec3(0.0f, 1.0f, 0.0f), const float &p_Yaw = s_Yaw, const float &p_Pitch = s_Pitch);
	Camera(const float &p_XPosition, const float &p_YPosition, const float &p_ZPosition, const float &p_XUp, const float &p_YUp, const float &p_ZUp, const float &p_Yaw, const float &p_Pitch);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(const CameraMovement &p_Direction, const float &p_DeltaTime);
	void ProcessMouseMovement(float p_XOffset, float p_YOffset, bool p_ContrainPitch = true);
};