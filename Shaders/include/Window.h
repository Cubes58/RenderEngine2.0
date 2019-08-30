#pragma once

#include <memory>
#include <vector>
#include <string>

struct GLFWwindow;

class Window {
private:
	static std::vector<bool> s_m_KeyPressBuffer;
	static std::vector<bool> s_m_KeyReleaseBuffer;
	static const int s_m_KeyBufferSize = 400;

	GLFWwindow *m_Window;
	static double s_m_MouseXPosition;
	static double s_m_MouseYPosition;
	static int s_m_ScreenWidth;
	static int s_m_ScreenHeight;
	static bool s_m_CaptureMouse;

	static void MouseMoveCallbackEvent(GLFWwindow *p_Window, double p_XPosition, double p_YPosition);
	static void CaptureMouse(GLFWwindow *p_Window);
	static void KeyCallbackEvent(GLFWwindow *p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Mods);
	static void WindowResizeCallbackEvent(GLFWwindow *p_Window, int p_Width, int p_Height);

public:
	Window() = default;
	~Window();

	bool InitializeWindow(int p_Width, int p_Height, const std::string &p_WindowName);

	GLFWwindow *GetWindow() const;

	static std::vector<bool> &KeyPressBuffer();
	static std::vector<bool> &KeyReleaseBuffer();

	static double MouseXPosition();
	static double MouseYPosition();

	static int Width();
	static int Height();
};