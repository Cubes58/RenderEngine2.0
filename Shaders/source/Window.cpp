#include "Window.h"

#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "ResourceManager.h"

std::vector<bool> Window::s_m_KeyPressBuffer;
std::vector<bool> Window::s_m_KeyReleaseBuffer;
int Window::s_m_ScreenWidth;
int Window::s_m_ScreenHeight;
double Window::s_m_MouseXPosition;
double Window::s_m_MouseYPosition;
bool Window::s_m_CaptureMouse = true;

void Window::MouseMoveCallbackEvent(GLFWwindow *p_Window, double p_XPosition, double p_YPosition) {
	s_m_MouseXPosition = p_XPosition;
	s_m_MouseYPosition = p_YPosition;
}

void Window::CaptureMouse(GLFWwindow *p_Window) {
	if (s_m_CaptureMouse)
		glfwSetInputMode(p_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Capture the mouse cursor.
	else
		glfwSetInputMode(p_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	// Release the mouse cursor.
}

void Window::KeyCallbackEvent(GLFWwindow *p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Mods) {
	if (p_Key == GLFW_KEY_UNKNOWN || p_Key > s_m_KeyBufferSize) {
		return;
	}
	s_m_KeyPressBuffer[p_Key] = ((p_Action == GLFW_PRESS || p_Action == GLFW_REPEAT));
	s_m_KeyReleaseBuffer[p_Key] = (p_Action == GLFW_RELEASE);

	if ((p_Key == GLFW_KEY_DELETE) && (p_Action == GLFW_RELEASE)) {
		s_m_CaptureMouse = !s_m_CaptureMouse;
		CaptureMouse(p_Window);
	}
	if (glfwGetKey(p_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(p_Window, true);
	}
}

void Window::WindowResizeCallbackEvent(GLFWwindow *p_Window, int p_Width, int p_Height) {
	// Change the OpenGL viewport to match the new window size.
	s_m_ScreenWidth = p_Width;
	s_m_ScreenHeight = p_Height;
	glViewport(0, 0, p_Width, p_Height);
}

bool Window::InitializeWindow(int p_Width, int p_Height, const std::string &p_WindowName) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	s_m_ScreenWidth = p_Width;
	s_m_ScreenHeight = p_Height;

	m_Window = glfwCreateWindow(p_Width, p_Height, p_WindowName.c_str(), nullptr, nullptr);
	if (m_Window == nullptr) {
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n" << "Version: " << glGetString(GL_VERSION);
	
	// Callback functions.
	glfwSetFramebufferSizeCallback(m_Window, WindowResizeCallbackEvent);
	glfwSetKeyCallback(m_Window, KeyCallbackEvent);

	// Mouse movement.
	glfwSetCursorPosCallback(m_Window, MouseMoveCallbackEvent);
	CaptureMouse(m_Window);

	// Make space for the keybuffer.
	s_m_KeyPressBuffer.resize(s_m_KeyBufferSize);
	std::fill(s_m_KeyPressBuffer.begin(), s_m_KeyPressBuffer.end(), false);

	s_m_KeyReleaseBuffer.resize(s_m_KeyBufferSize);
	std::fill(s_m_KeyReleaseBuffer.begin(), s_m_KeyReleaseBuffer.end(), false);

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	// Enable alpha transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable face culling.
	glEnable(GL_CULL_FACE);

	// Initialize the static resource manager instance.
	ResourceManagerInstance;

	return true;
}

Window::~Window() {
	// Clean up.
	glfwTerminate();
}

GLFWwindow *Window::GetWindow() const {
	return m_Window;
}

std::vector<bool> &Window::KeyPressBuffer() {
	return s_m_KeyPressBuffer;
}

std::vector<bool> &Window::KeyReleaseBuffer() {
	return s_m_KeyReleaseBuffer;
}

double Window::MouseXPosition() {
	return s_m_MouseXPosition;
}

double Window::MouseYPosition() {
	return s_m_MouseYPosition;
}

int Window::Width() {
	return s_m_ScreenWidth;
}

int Window::Height() {
	return s_m_ScreenHeight;
}