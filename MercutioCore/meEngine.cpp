#include "stdafx.h"
#include "meEngine.h"
#include <time.h>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include "meIni.h"

void Engine::TerminalOptionsMenu()
{
	std::cout << "OPTIONS\n\n";
	bool inOptions = true;
	while (inOptions)
	{
		std::cout << "[r] Resolution: " << displayManager.GetCurrentMode()->width <<
			" x " << displayManager.GetCurrentMode()->height << "\n";
		std::cout << "[m] Monitor: " << displayManager.GetCurrentMode()->monitor << "\n";
		std::cout << "[w] Window Mode: ";
		if (displayManager.GetCurrentMode()->mode == DisplayMode::window)
		{
			std::cout << "Windowed\n";
		}
		else
		{
			std::cout << "Fullscreen\n";
		}
		std::cout << "[e] Exit and Resume game\n";
		std::cout << "Any other input Exits and Quits\n";
		size_t i;
		std::string in;
		char input;
		std::cin >> input;
		size_t inputIndex;
		
		switch (input)
		{
		case('r') :
			std::cout << displayManager.GetResolutions()->size();
			for (i = 0; i < displayManager.GetResolutions()->size(); i++)
			{
				std::cout << "[" << i << "] " << displayManager.GetResolutions()->at(i) << "\n";
			}
				  std::cin >> in;
				  try
				  {
					  inputIndex = std::stoi(in);
					  displayManager.SetResolution(inputIndex);
				  }
				  catch (std::invalid_argument &e)
				  {
					  (void)e;
					  std::cout << "Invalid input.\n";
				  }
			break;
		case('m') :
			for (i = 0; i < displayManager.GetResolutions()->size(); i++)
			{
				std::cout << "[" << i << "] ";
			}
				  std::cout << "\n";
				  std::cin >> in;
				  try
				  {
					  inputIndex = std::stoi(in);
					  displayManager.SetMonitor(inputIndex);
				  }
				  catch (std::invalid_argument &e)
				  {
					  (void)e;
					  std::cout << "Invalid input.\n";
				  }
			break;
		case('w') :
			for (i = 0; i < displayManager.GetResolutions()->size(); i++)
			{
				std::cout << "[0] windowed [1] fullscreen\n";
			}
				  std::cin >> in;
				  try
				  {
					  inputIndex = std::stoi(in);
					  displayManager.SetMode(inputIndex);
				  }
				  catch (std::invalid_argument &e)
				  {
					  (void)e;
					  std::cout << "Invalid input.\n";
				  }
			break;
		case('e') :
			inOptions = false;
			break;
		default:
			inOptions = false;
			gameRunning = false;
			break;
		}
		displayManager.StageChanges();
		displayManager.SaveChanges();
	}
}

void Engine::Launch()
{
	glfwInit();
	//if settings.ini does not exist create it

	//read settings.ini
	Ini settings;
	settings.Load("gamedata/settings.ini");
	displayManager.Init(&settings);

	//start game loop
	while (gameRunning)
	{
		Update();
		glfwPollEvents();
		if (glfwGetKey(displayManager.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			//gameRunning = false;
			TerminalOptionsMenu();
		}
		glfwSwapBuffers(displayManager.GetWindow());
	}
	
	glfwTerminate();
}
void Engine::Update()
{
	//
	std::vector<Entity*>::iterator i;
	deltaTime = lastFrame - clock.Time();
	lastFrame = clock.Time();
	bool fixedUpdate = false;
	if (lastFixedUpdate - clock.Time() > fixedUpdateRate)
	{
		lastFixedUpdate = clock.Time();
		fixedUpdate = true;
	}
	for(i = entities.begin(); i != entities.end(); ++i)
	{
		if ((*i)->isFreed)
		{
			entities.erase(i);
			continue;
		}
		(*i)->Update();
		//set each entity's deltatime to the engine's value
		//technically slower than having all entities hold a pointer to the value
		//but this is more readable
		(*i)->deltaTime = deltaTime; 
		
		if (fixedUpdate)
		{
			(*i)->FixedUpdate();
		}
	}
}

Engine::Engine()
{
}


Engine::~Engine()
{
}
