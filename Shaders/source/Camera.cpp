#include "Camera.h"

Camera::Camera(const glm::vec3 &p_Position, const glm::vec3 &p_Up, const float &p_Yaw, const float &p_Pitch) :
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(s_Speed), m_MouseSensitivity(s_Sensitivity), m_Zoom(s_Zoom) {
	m_Position = p_Position;
	m_WorldUp = p_Up;
	m_Yaw = p_Yaw;
	m_Pitch = p_Pitch;

	UpdateCameraVectors();
}

Camera::Camera(const float &p_XPosition, const float &p_YPosition, const float &p_ZPosition, const float &p_XUp, const float &p_YUp, const float &p_ZUp, const float &p_Yaw, const float &p_Pitch) :
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(s_Speed), m_MouseSensitivity(s_Sensitivity), m_Zoom(s_Zoom) {

	m_Position = glm::vec3(p_XPosition, p_YPosition, p_ZPosition);
	m_WorldUp = glm::vec3(p_XUp, p_YUp, p_ZUp);
	m_Yaw = p_Yaw;
	m_Pitch = p_Pitch;

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessKeyboard(const CameraMovement &p_Direction, const float &p_DeltaTime) {
	float velocity = m_MovementSpeed * p_DeltaTime;
	if (m_EnableSpeedBoost)
		velocity *= m_BoostSpeedMultiplayer;

	if (p_Direction == CameraMovement::FORWARD)
		m_Position += m_Front * velocity;
	if (p_Direction == CameraMovement::BACKWARD)
		m_Position -= m_Front * velocity;
	if (p_Direction == CameraMovement::LEFT)
		m_Position -= m_Right * velocity;
	if (p_Direction == CameraMovement::RIGHT)
		m_Position += m_Right * velocity;
	if (p_Direction == CameraMovement::UP)
		m_Position += m_Up * velocity;
	if (p_Direction == CameraMovement::DOWN)
		m_Position -= m_Up * velocity;
}

void Camera::ProcessMouseMovement(float p_XOffset, float p_YOffset, bool p_ContrainPitch) {
	p_XOffset *= m_MouseSensitivity;
	p_YOffset *= m_MouseSensitivity;

	m_Yaw += (p_XOffset * -1);	// Stop inversion of the mouse movement.
	m_Pitch += p_YOffset;

	NormalizeAngle();
	UpdateCameraVectors();
}

void Camera::NormalizeAngle() {
	if (this->m_Pitch > s_MaxPitchAngle)
		this->m_Pitch = s_MaxPitchAngle;

	if (this->m_Pitch < -s_MaxPitchAngle)
		this->m_Pitch = -s_MaxPitchAngle;

	if (this->m_Yaw < 0.0f)
		this->m_Yaw += 360.0f;
}

void Camera::UpdateCameraVectors() {
	glm::vec3 front;
	front.x = -sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = -cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	glm::vec3 side;
	side.x = cos(glm::radians(m_Yaw));
	side.y = 0;
	side.z = -sin(glm::radians(m_Yaw));
	m_Right = glm::normalize(side);
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}