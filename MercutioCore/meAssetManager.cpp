#include "stdafx.h"
#include "meAssetManager.h"

void AssetManager::Init()
{
	manifest.Load("gamedata/asset_manifest.ini");
}
void AssetManager::LoadPage(std::string name)
{
	//
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
