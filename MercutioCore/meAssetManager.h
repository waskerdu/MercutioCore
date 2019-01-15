#pragma once
#include <string>
#include <map>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include "meRenderable.h"
#include "meIni.h"

struct Asset
{
	bool isLoaded = false;
	bool isNeeded = false;
	std::vector<std::string> instructions;
	std::string name;
	void* data;
	Asset(){}
	~Asset()
	{
		delete data;
	}
};

class AssetManager
{
	Ini manifest;
	std::map<std::string, Asset> meshes;
	std::map<std::string, Asset> materials;
	std::map<std::string, Asset> renderables;
	std::map<std::string, Asset> textures;
	std::map<std::string, Asset> shaders;
	/*std::map<std::string, Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::map<std::string, Renderable*> renderables;
	std::map<std::string, GLuint> shaders;
	std::map<std::string, GLuint> textures;*/
	void RegisterAssets();
	void SetNotNeeded(std::map<std::string, Asset>* map);
	void UnloadUnneeded(std::map<std::string, Asset>* map);
	std::string GetExtension(std::string filename);
	
	bool LoadShader(std::string alias);
	//bool UnloadShader(std::string alias);
	bool CompileShader(unsigned int* handle, std::string filename, unsigned int type);
	const char* GetShaderCode(std::string filename);

	bool LoadTexture(std::string alias);
	//bool UnloadTexture(std::string alias);

	//bool LoadMesh(std::string alias);
	//bool UnloadMesh(std::string alias);

	bool LoadMaterial(std::string alias);
	//bool UnloadMaterial(std::string alias);

	bool LoadRenderable(std::string alias);
	//bool UnloadRenderable(std::string alias);

public:
	void Init();
	bool LoadPage(std::string name);
	Renderable* GetRenderable(std::string name);
	//sound effect
	//music
	AssetManager();
	~AssetManager();
};

