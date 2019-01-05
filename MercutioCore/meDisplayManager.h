#pragma once
#include <vector>
#include <string>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include "meIni.h"

struct DisplayMode
{
	enum Mode
	{
		window,
		fullscreen,
		borderless,
	};
	Mode mode = window;
	size_t width = 640;
	size_t height = 480;
	size_t monitor = 0;
	double refreshRate = 60.0;
};

class DisplayManager
{
	/*
	Manages the window system
	*/
	GLFWwindow* window;
	DisplayMode mode;
	DisplayMode tempMode;
	DisplayMode lastChanceMode;
	//std::vector<DisplayMode> avalibleModes;
	GLFWmonitor** monitors;
	std::vector<std::pair<size_t, size_t>> resolutions;
	std::vector<std::string> resolutionStrings;
	std::vector<size_t> monitorIndicies;
	std::vector<std::string> monitorStrings;
	std::vector<double> refreshRates;
	std::vector<std::string> refreshRateStrings;
	//the style guide highly recommends integer sizes to be explicit
	//in this case it should also be unsigned
	//unfortunately that's not what glfw wants
	int numMonitors = 0; 
	Ini* ini = nullptr;
public:
	//void Init(bool windowed = true, size_t x = 640, size_t y = 480, 
		//size_t monitor = 0, const char* title = "Mercutio");
	void Init(Ini* ini);
	//void SetMode(DisplayMode* mode);
	GLFWwindow* GetWindow();
	std::vector<std::string>* GetResolutions();
	std::vector<std::string>* GetMonitors();
	std::vector<std::string>* GetRefreshRates();
	void SetResolution(size_t index);
	void SetMonitor(size_t index);
	void SetRefreshRate(size_t index);
	void SetMode(size_t index);
	void SaveChanges();
	void StageChanges();
	DisplayMode* GetCurrentMode();
	DisplayManager();
	~DisplayManager();
};

