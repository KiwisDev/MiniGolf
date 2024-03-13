#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <iostream>
#include <Windows.h>

#include "shader.h"
#include "font_renderer.h"
#include "ressource_manager.h"

// Settings

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define FPS 60

// Variables

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

FontRenderer* poppins = nullptr;

int main() {
	// Init

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Mini Golf", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		return  -1;
	}
	glfwSetWindowAspectRatio(window, 16, 9);
	glfwSetWindowSizeLimits(window, DEFAULT_WIDTH, DEFAULT_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	// Init font
	Shader* font_shader = RessourceManager::loadShader("font_shader", "shaders/fontVertex.vs", "shaders/fontFragment.fs");
	poppins = new FontRenderer(font_shader, "fonts/Poppins-Regular.ttf", DEFAULT_WIDTH, DEFAULT_HEIGHT);

	while (!glfwWindowShouldClose(window)) {
		// -- FPS Limiter -- //
		while (glfwGetTime() < lastFrame + 1.0 / FPS) {
			Sleep((lastFrame + 1.0 / FPS) - glfwGetTime());
		}

		// -- Delta time -- //
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// -- User input -- //

		// -- Physics -- //

		// -- Render -- //
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render FPS
		poppins->renderText(std::to_string((int) (1 / deltaTime)), 0, 0, 1, glm::vec3(255));

		// -- Events + Buffer -- //
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	// -- Deletes -- //

	delete poppins;

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// -- Debug -- //
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		GLint polyMode[2];
		glGetIntegerv(GL_POLYGON_MODE, &(*polyMode));

		if (polyMode[0] == GL_FILL) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	// -- Game related -- //

	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
		if (glfwGetWindowMonitor(window) == NULL) {
			int monitor_response = 0;
			GLFWmonitor* monitor = *(glfwGetMonitors(&monitor_response));
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			assert(monitor_response != 0);

			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			poppins->resize(mode->width, mode->height);
		}
		else {
			glfwSetWindowMonitor(window, NULL, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0);
			poppins->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	poppins->resize(width, height);
}