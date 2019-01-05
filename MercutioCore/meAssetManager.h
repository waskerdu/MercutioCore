#pragma once
#include <string>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

class AssetManager
{
public:
	void Init();
	void LoadPage(std::string name);
	void* GetMesh(std::string name);
	void* GetMaterial(std::string name);
	AssetManager();
	~AssetManager();
};

