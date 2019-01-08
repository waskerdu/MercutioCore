#include "stdafx.h"
#include "meAssetManager.h"

void AssetManager::Init()
{
	manifest.Load("gamedata/asset_manifest.ini");
}
void AssetManager::LoadPage(std::string name)
{
	std::vector<std::vector<std::string>> data;
	std::vector<std::vector<std::string>> meshEntries;
	std::vector<std::vector<std::string>> materialEntries;
	std::vector<std::vector<std::string>> renderableEntries;
	std::vector<std::vector<std::string>> shaderEntries;

	manifest.GetSection(name, &data);
	size_t i;
	for (i = 0; i < data.size(); i++)
	{
		if (data[i][1] == "mesh")
		{
			//meshes.insert(std::pair<std::string, Mesh*>(data[i][0], nullptr));
			meshEntries.push_back(data[i]);
		}
		else if (data[i][1] == "material")
		{
			//materials.insert(std::pair<std::string, Material*>(data[i][0], nullptr));
			materialEntries.push_back(data[i]);
		}
		if (data[i][1] == "renderable")
		{
			//renderables.insert(std::pair<std::string, Renderable*>(data[i][0], nullptr));
			renderableEntries.push_back(data[i]);
		}
		if (data[i][1] == "shader")
		{
			//renderables.insert(std::pair<std::string, Renderable*>(data[i][0], nullptr));
			shaderEntries.push_back(data[i]);
		}
	}

	for (i = 0; i < meshEntries.size(); i++)
	{
		//
	}
	for (i = 0; i < shaderEntries.size(); i++)
	{
		//
	}
	for (i = 0; i < materialEntries.size(); i++)
	{
		//
	}
}
Mesh* AssetManager::GetMesh(std::string name)
{
	return nullptr;
}
Material* AssetManager::GetMaterial(std::string name)
{
	return nullptr;
}
Renderable* AssetManager::GetRenderable(std::string name)
{
	return nullptr;
}

AssetManager::AssetManager()
{
}


AssetManager::~AssetManager()
{
}
