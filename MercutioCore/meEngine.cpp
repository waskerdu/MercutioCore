#include "stdafx.h"
#include "meEngine.h"
#include <time.h>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

void Engine::Launch()
{
	glfwInit();
	testWindow = glfwCreateWindow(640, 480, "test window", NULL, NULL);
	//read init.ini

	//start game loop
	while (gameRunning)
	{
		Update();
		glfwPollEvents();
		if (glfwGetKey((GLFWwindow*)testWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			gameRunning = false;
		}
		glfwSwapBuffers((GLFWwindow*)testWindow);
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
