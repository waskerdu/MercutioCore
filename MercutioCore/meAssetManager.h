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
	std::map<std::string, GLuint> shaders;
	std::map<std::string, GLuint> textures;
	std::string GetExtension(std::string filename);
	const char* GetShaderCode(std::string filename);
	bool CreateShader(std::vector<std::string>* instructions);
	bool CompileShader(unsigned int* handle, std::string filename, unsigned int type);
	bool CreateTexture(std::vector<std::string>* instructions);
	//bool CreateMaterial(std::vector<std::string>* instructions);
public:
	void Init();
	bool LoadPage(std::string name);
	Mesh* GetMesh(std::string name);
	Material* GetMaterial(std::string name);
	Renderable* GetRenderable(std::string name);
	//sound effect
	//music
	AssetManager();
	~AssetManager();
};

