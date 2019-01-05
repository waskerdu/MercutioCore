#pragma once
#include <vector>
#include "meClock.h"
#include "meEntity.h"
#include "meMemoryManager.h"
#include "meAssetManager.h"
#include "meDisplayManager.h"
#include "meRenderer.h"
class Engine
{
	/*
	The Engine class is the beating heart of Mecutio
	"All" it does is manages the entities in the scene and calls the various managers when needed
	*/
	//private fields
	Clock clock;
	MemoryManager memoryManager;
	AssetManager assetManager;
	DisplayManager displayManager;
	Renderer renderer;

	bool gameRunning = true;

	std::vector<Entity*> entities;
	double timeSinceLaunch = 0.0;
	double deltaTime = 0.0;
	double lastFrame = 0.0;
	double lastFixedUpdate = 0.0;
	double fixedUpdateRate = 0.2;

	//void* testWindow;

public:
	//public fields (minimize/eliminate)

private:
	//private methods
	void TerminalOptionsMenu();

protected:
	//protected methods
public:
	//public methods
	void Launch();
	void Update();

	//constructor / destructor
	Engine();
	~Engine();
};

