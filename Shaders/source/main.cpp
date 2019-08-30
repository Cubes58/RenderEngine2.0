#include <glfw/glfw3.h>

#include "Scene.h"
#include "Window.h"

int main() {
	std::shared_ptr<Window> window = std::make_shared<Window>();
	if (!window->InitializeWindow(800, 600, "Advanced Shaders"))
		return -1;

	std::shared_ptr<Scene> scene = std::make_shared<Scene>(window);

	auto previousTime = glfwGetTime();
	while (!glfwWindowShouldClose(window->GetWindow()) && scene->IsRunning()) {
		auto currentTime = glfwGetTime();
		auto deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		scene->HandleMouseInput(static_cast<float>(window->MouseXPosition()), static_cast<float>(window->MouseYPosition()));
		scene->HandleKeyboardInput(window->KeyPressBuffer(), window->KeyReleaseBuffer());

		scene->Update(static_cast<float>(deltaTime));
		scene->Render();

		// Swap the buffers.
		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	return 0;
}