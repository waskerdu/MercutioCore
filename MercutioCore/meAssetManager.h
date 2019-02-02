#pragma once
#include <string>
#include <map>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include "meRenderable.h"
#include "meIni.h"
#include "meAsset.h"

class AssetManager
{
	Ini manifest;
	std::map<std::string, Asset> meshes;
	std::map<std::string, Asset> materials;
	std::map<std::string, Asset> textures;
	std::map<std::string, Asset> shaders;
	void RegisterAssets();
	void SetNotNeeded(std::map<std::string, Asset>* map);
	void UnloadUnneeded(std::map<std::string, Asset>* map);
	//void Free(std::map<std::string, Asset>* map);
	std::string GetExtension(std::string filename);
	bool LoadShader(std::string alias);
	bool CompileShader(unsigned int* handle, std::string filename, unsigned int type);
	const char* GetShaderCode(std::string filename);
	bool LoadTexture(std::string alias);
	bool LoadMaterial(std::string alias);
	bool LoadMesh(std::string alias);
	//bool LoadRenderable(std::string alias);

public:
	void Init();
	void SetNotNeeded();
	bool LoadPage(std::string name);
	void UnloadUnneeded();
	bool GetRenderable(Renderable* renderable, std::string materialName, std::string meshName);
	//sound effect
	//music
	AssetManager();
	~AssetManager();
};

