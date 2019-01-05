#pragma once
#include <vector>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

struct DisplayMode
{
	enum Mode
	{
		window,
		fullscreen,
		borderless,
	};
	Mode mode = window;
	size_t x = 640;
	size_t y = 480;
	double refreshRate = 60.0;
};

class DisplayManager
{
	/*
	Manages the window system
	*/
	GLFWwindow* window;
	DisplayMode mode;
	std::vector<DisplayMode> avalibleModes;
	GLFWmonitor** monitors;
	//the style guide highly recommends integer sizes to be explicit
	//in this case it should also be unsigned
	//unfortunately that's not what glfw wants
	int numMonitors = 0; 
public:
	void Init(bool windowed = true, size_t x = 640, size_t y = 480, 
		size_t monitor = 0, const char* title = "Mercutio");
	void SetMode(DisplayMode* mode);
	GLFWwindow* GetWindow();
	std::vector<DisplayMode>* GetModes();
	DisplayMode* GetMode();
	size_t GetWindowWidth();
	size_t GetWindowHeight();
	DisplayManager();
	~DisplayManager();
};

