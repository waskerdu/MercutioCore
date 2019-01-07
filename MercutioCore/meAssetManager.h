#pragma once
#include <string>
#include <map>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include "meRenderable.h"
#include "meIni.h"

class AssetManager
{
	Ini manifest;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::map<std::string, Renderable*> renderables;
public:
	void Init();
	void LoadPage(std::string name);
	Mesh* GetMesh(std::string name);
	Material* GetMaterial(std::string name);
	Renderable* GetRenderable(std::string name);
	//sound effect
	//music
	AssetManager();
	~AssetManager();
};

