#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../include/game/game.h"
#include "../include/io/Keyboard.h"
#include "../include/io/Mouse.h"
#include "../include/memory/ResourceManager.h"

#include <iostream>

void processInput(GLFWwindow* window, float dt);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

Game Breakout(SCR_WIDTH, SCR_HEIGHT);

int main() {
	Paths::init();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initiliaze GLAD" << std::endl;
		return -1;
	}
	
	// set callbacks
	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


	// OpenGL configuration
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Breakout.init();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// manage user input
		processInput(window, deltaTime);
		Breakout.processInput(deltaTime);

		// update game state
		Breakout.update(deltaTime);

		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.render();

		glfwSwapBuffers(window);
	}
	// delete all resources as loaded by resource manager
	ResourceManager::clear();

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window, float dt){
	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){

	glViewport(0, 0, width, height);
}
