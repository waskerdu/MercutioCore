#include "stdafx.h"
#include "meDisplayManager.h"
#include <iostream>

void DisplayManager::Init(Ini* ini)
{
	this->ini = ini;
	if (ini->Get("window", "mode") == "windowed")
	{
		mode.mode = DisplayMode::window;
	}
	else
	{
		mode.mode = DisplayMode::fullscreen;
	}
	mode.width = (size_t)ini->GetInt("window", "width");
	mode.height = (size_t)ini->GetInt("window", "height");
	mode.monitor = (size_t)ini->GetInt("window", "monitor");
	mode.refreshRate = ini->GetDouble("window", "refreshRate");
	tempMode = mode;
	lastChanceMode = mode;
	std::string title = ini->Get("window", "title");
	monitors = glfwGetMonitors(&numMonitors);
	int count;
	const GLFWvidmode* modes;
	for (size_t i = 0; i < (size_t)numMonitors; i++)
	{
		modes = glfwGetVideoModes(monitors[i], &count);
		monitorIndicies.push_back(i);
		monitorStrings.push_back(std::to_string(i));
		refreshRates.push_back(modes[0].refreshRate);
		refreshRateStrings.push_back(std::to_string(modes[0].refreshRate));
		resolutions.push_back(std::pair<size_t, size_t>(modes[0].width, modes[0].height));
		resolutionStrings.push_back(std::to_string(modes[0].width) + " " + std::to_string(modes[0].height));
		for (size_t f = 1; f < (size_t)count; f++)
		{
			if (refreshRates.back() != modes[f].refreshRate)
			{
				refreshRates.push_back(modes[f].refreshRate);
				refreshRateStrings.push_back(std::to_string(modes[f].refreshRate));
			}
			if (resolutions.back() != std::pair<size_t, size_t>(modes[f].width, modes[f].height))
			{
				resolutions.push_back(std::pair<size_t, size_t>(modes[f].width, modes[f].height));
				resolutionStrings.push_back(std::to_string(modes[f].width) + " " + std::to_string(modes[f].height));
			}
		}
	}
	if (mode.mode == DisplayMode::window)
	{
		window = glfwCreateWindow(mode.width, mode.height, title.c_str(), NULL, NULL);
	}
	else
	{
		window = glfwCreateWindow(mode.width, mode.height, title.c_str(), monitors[mode.monitor], NULL);
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, mode.width, mode.height);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "OpenGl Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Vendor: %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "Renderer: %s\n", glGetString(GL_RENDERER));
}

GLFWwindow* DisplayManager::GetWindow()
{
	return window;
}

std::vector<std::string>* DisplayManager::GetResolutions()
{
	return &resolutionStrings;
}
std::vector<std::string>* DisplayManager::GetMonitors()
{
	return &monitorStrings;
}
std::vector<std::string>* DisplayManager::GetRefreshRates()
{
	return &refreshRateStrings;
}

void DisplayManager::SetResolution(size_t index)
{
	tempMode.width = resolutions[index].first;
	tempMode.height = resolutions[index].second;
}
void DisplayManager::SetMonitor(size_t index)
{
	tempMode.monitor = index;
}
void DisplayManager::SetRefreshRate(size_t index)
{
	tempMode.refreshRate = refreshRates[index];
}
void DisplayManager::SetMode(size_t index)
{
	if (index == 0)
	{
		tempMode.mode = DisplayMode::window;
	}
	else
	{
		tempMode.mode = DisplayMode::fullscreen;
	}
}
void DisplayManager::SaveChanges()
{
	lastChanceMode = mode;
	if (mode.mode == DisplayMode::window)
	{
		ini->Set("window", "mode", "windowed");
	}
	else
	{
		ini->Set("window", "mode", "fullscreen");
	}
	ini->Set("window", "width", (int64_t)mode.width);
	ini->Set("window", "height", (int64_t)mode.height);
	ini->Set("window", "monitor", (int64_t)mode.monitor);
	ini->Set("window", "refreshRate", (double)mode.refreshRate);
	ini->Save();
}
void DisplayManager::StageChanges()
{
	mode = tempMode;
	glViewport(0, 0, mode.width, mode.height);
	if (mode.mode == DisplayMode::window)
	{
		glfwSetWindowMonitor(window, NULL, 100, 100, mode.width, mode.height, (int)mode.refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(window, monitors[mode.monitor], 100, 100, mode.width, mode.height, (int)mode.refreshRate);
	}
}
DisplayMode* DisplayManager::GetCurrentMode()
{
	return &mode;
}

DisplayManager::DisplayManager()
{
}


DisplayManager::~DisplayManager()
{
}
