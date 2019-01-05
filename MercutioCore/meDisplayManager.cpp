#include "stdafx.h"
#include "meDisplayManager.h"

void DisplayManager::Init(bool windowed, size_t x, size_t y, size_t monitor, const char* title)
{
	//
	monitors = glfwGetMonitors(&numMonitors);
	if (windowed)
	{
		window = glfwCreateWindow(x, y, title, NULL, NULL);
		mode.x = x;
		mode.y = y;
		mode.mode = DisplayMode::window;
	}
	else
	{
		window = glfwCreateWindow(x, y, title, monitors[monitor], NULL);
		mode.x = x;
		mode.y = y;
		mode.mode = DisplayMode::fullscreen;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, x, y);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "OpenGl Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Vendor: %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "Renderer: %s\n", glGetString(GL_RENDERER));
}

void DisplayManager::SetMode(DisplayMode* mode)
{
	//
}

GLFWwindow* DisplayManager::GetWindow()
{
	return window;
}

std::vector<DisplayMode>* DisplayManager::GetModes()
{
	return &avalibleModes;
}

DisplayMode* DisplayManager::GetMode()
{
	return &mode;
}
size_t DisplayManager::GetWindowWidth()
{
	return mode.x;
}

size_t DisplayManager::GetWindowHeight()
{
	return mode.y;
}

DisplayManager::DisplayManager()
{
}


DisplayManager::~DisplayManager()
{
}
