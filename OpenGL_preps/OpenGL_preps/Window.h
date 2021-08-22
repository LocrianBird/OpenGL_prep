#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma once
class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int initialize();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};