#include "stdafx.h"
#include "meAssetManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "OBJ_Loader.h"

void AssetManager::RegisterAssets()
{
	// collates all the assets in the game and prepares datastructures for them

	// first generate all default assets (mesh, texture, shader, material)
	std::vector<float> verts
	{
		//positions             //normals			    //texture coords
		50.0f,  50.0f, 0.0f,   1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		50.0f, -50.0f, 0.0f,   0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		-50.0f, -50.0f, 0.0f,   0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		-50.0f,  50.0f, 0.0f,   1.0f, 1.0f, 0.0f,		0.0f, 1.0f
	};
	std::vector<unsigned int> indices{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	Mesh* quadMesh = new Mesh(verts, indices);
	Asset quadAsset = Asset();
	quadAsset.data = quadMesh;
	quadAsset.isLoaded = true;
	quadAsset.isNeeded = true;
	meshes.insert(std::pair<std::string, Asset>("default", quadAsset));

	std::vector<std::vector<std::string>> data;
	manifest.GetSection("game_assets", &data);
	Asset asset = Asset();
	for (size_t i = 0; i < data.size(); i++)
	{
		asset.instructions = data[i];
		asset.name = asset.instructions[0];
		if (data[i][1] == "mesh")
		{
			meshes.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
		else if (data[i][1] == "material")
		{
			materials.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
		else if (data[i][1] == "texture")
		{
			textures.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
		else if (data[i][1] == "shader")
		{
			shaders.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
	}
}

void AssetManager::SetNotNeeded(std::map<std::string, Asset>* map)
{
	// mark all non-default objects as not needed
	// utility method used to refresh needed assets
	std::map<std::string, Asset>::iterator i;
	for (i = map->begin(); i != map->end(); i++)
	{
		i->second.isNeeded = false;
	}
	if (map->count("default") != 0)
	{
		map->at("default").isNeeded = true;
	}
	//should just be this:
	// map->at("default").isNeeded = true;
	//but don't have all the defaults in yet.
}

void AssetManager::UnloadUnneeded(std::map<std::string, Asset>* map)
{
	// free memory of assets we don't need right now
	std::map<std::string, Asset>::iterator i;
	for (i = map->begin(); i != map->end(); i++)
	{
		if (i->second.isNeeded == false)
		{
			std::string alias = i->second.name;
			map->at(alias).isLoaded = false;
			delete map->at(alias).data;
			map->at(alias).data = nullptr;
		}
	}
}

std::string AssetManager::GetExtension(std::string filename)
{
	size_t start = filename.find(".");
	if (start == std::string::npos) { return ""; }
	return filename.substr(start + 1);
}

bool AssetManager::LoadShader(std::string alias)
{
	if (shaders.count(alias) == 0)
	{
		std::cout << "No shader named " << alias << " exists.\n";
		return false;
	}
	if (shaders[alias].isLoaded) 
	{
		return true;
	}
	std::vector<std::string>* instructions = &shaders[alias].instructions;
	int success;
	char infoLog[512];
	GLuint id;
	size_t i;
	unsigned int shaderHandle;
	id = glCreateProgram();
	for (i = 0; i < instructions->size(); i++)
	{
		if (GetExtension(instructions->at(i)) == "vert")
		{
			if (CompileShader(&shaderHandle, instructions->at(i), GL_VERTEX_SHADER))
			{
				glAttachShader(id, shaderHandle);
				glDeleteShader(shaderHandle);
				break;
			}
		}
	}
	for (i = 0; i < instructions->size(); i++)
	{
		if (GetExtension(instructions->at(i)) == "frag")
		{
			if (CompileShader(&shaderHandle, instructions->at(i), GL_FRAGMENT_SHADER))
			{
				glAttachShader(id, shaderHandle);
				glDeleteShader(shaderHandle);
				break;
			}
		}
	}
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}
	shaders[alias].data = new GLuint(id);
	shaders[alias].isLoaded = true;
	shaders[alias].isNeeded = true;
	return true;
}

const char* AssetManager::GetShaderCode(std::string filename)
{
	std::ifstream file;
	std::string code;
	std::stringstream stream;
	file.open(filename);
	stream << file.rdbuf();
	file.close();
	code = stream.str();
	return code.c_str();
}

bool AssetManager::CompileShader(unsigned int* handle, std::string filename, unsigned int type)
{
	*handle = glCreateShader(type);
	int success;
	char infoLog[512];
	const char* code = GetShaderCode(filename);
	glShaderSource(*handle, 1, &code, NULL);
	glCompileShader(*handle);
	glGetShaderiv(*handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*handle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	};
	return true;
}

bool AssetManager::LoadTexture(std::string alias)
{
	if (textures.count(alias) == 0)
	{
		std::cout << "No texture named " << alias << " exists.\n";
		return false;
	}
	if (textures[alias].isLoaded)
	{
		return true;
	}
	GLuint id = 0;
	std::vector<std::string>* instructions = &textures[alias].instructions;
	std::string filename = instructions->at(2);
	unsigned char* textureData;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	textureData = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << filename << " failed to load.\n";
		return false;
	}
	stbi_image_free(textureData);
	textures[alias].data = new GLuint(id);
	textures[alias].isLoaded = true;
	textures[alias].isNeeded = true;
	return true;
}

bool AssetManager::LoadMaterial(std::string alias)
{
	if (materials.count(alias) == 0)
	{
		std::cout << "No material named " << alias << " exists.\n";
		return false;
	}
	if (materials[alias].isLoaded)
	{
		return true;
	}
	std::vector<std::string>* instructions = &materials[alias].instructions;

	if (textures.count(instructions->at(2)) == 0) 
	{ 
		std::cout << "Failed to find texture " << instructions->at(2) << "\n";
		return false; 
	}
	if (textures[instructions->at(2)].isLoaded == false)
	{
		std::cout << "Texture " << instructions->at(2) << " is not loaded.\n";
		if (!LoadTexture(instructions->at(2))) { return false; }
	}
	if (shaders.count(instructions->at(3)) == 0)
	{
		std::cout << "Failed to find shader " << instructions->at(3) << "\n";
		return false;
	}
	if (shaders[instructions->at(3)].isLoaded == false)
	{
		std::cout << "Shader " << instructions->at(3) << " is not loaded.\n";
		LoadShader(instructions->at(3));
	}
	GLuint* texture = (GLuint*)textures[instructions->at(2)].data;
	GLuint* shader = (GLuint*)shaders[instructions->at(3)].data;
	materials[alias].data = new Material(*texture, *shader);
	materials[alias].isLoaded = true;
	materials[alias].isNeeded = true;
	return true;
}

bool AssetManager::LoadMesh(std::string alias)
{
	if (meshes.count(alias) == 0)
	{
		std::cout << "No mesh named " << alias << " exists.\n";
		return false;
	}
	if (meshes[alias].isLoaded)
	{
		return true;
	}
	std::vector<std::string>* instructions = &meshes[alias].instructions;

	objl::Loader loader;
	loader.LoadFile(instructions->at(2));
	std::vector<float> verts;
	std::vector<unsigned int> indicies;
	for (size_t i = 0; i < loader.LoadedVertices.size(); i++)
	{
		verts.push_back(loader.LoadedVertices[i].Position.X);
		verts.push_back(loader.LoadedVertices[i].Position.Y);
		verts.push_back(loader.LoadedVertices[i].Position.Z);
		verts.push_back(loader.LoadedVertices[i].Normal.X);
		verts.push_back(loader.LoadedVertices[i].Normal.Y);
		verts.push_back(loader.LoadedVertices[i].Normal.Z);
		verts.push_back(loader.LoadedVertices[i].TextureCoordinate.X);
		verts.push_back(loader.LoadedVertices[i].TextureCoordinate.Y);
	}

	for (size_t i = 0; i < loader.LoadedIndices.size(); i++)
	{
		indicies.push_back(loader.LoadedIndices[i]);
	}
	
	meshes[alias].data = new Mesh(verts, indicies);
	meshes[alias].isLoaded = true;
	meshes[alias].isNeeded = true;
	return true;
}

void AssetManager::Init()
{
	manifest.Load("gamedata/asset_manifest.ini");
	//register assets
	RegisterAssets();
	//load minimal assets
	LoadPage("init");
}

void AssetManager::SetNotNeeded()
{
	// calls SetNotNeeded on all assets
	SetNotNeeded(&meshes);
	SetNotNeeded(&materials);
	SetNotNeeded(&textures);
	SetNotNeeded(&shaders);
}

bool AssetManager::LoadPage(std::string pageName)
{
	//for all assets in the page set them to needed and load them
	std::vector<std::vector<std::string>> data;
	manifest.GetSection(pageName, &data);

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i][0] == "meshes")
		{
			for (size_t f = 1; f < data[i].size(); f++)
			{
				LoadMesh(data[i][f]);
			}
		}
		if (data[i][0] == "textures")
		{
			for (size_t f = 1; f < data[i].size(); f++)
			{
				LoadTexture(data[i][f]);
			}
		}
		if (data[i][0] == "shaders")
		{
			for (size_t f = 1; f < data[i].size(); f++)
			{
				LoadShader(data[i][f]);
			}
		}
		if (data[i][0] == "materials")
		{
			for (size_t f = 1; f < data[i].size(); f++)
			{
				LoadMaterial(data[i][f]);
			}
		}
	}
	return true;
}

void AssetManager::UnloadUnneeded()
{
	//for all assets if they are not needed unload them
	UnloadUnneeded(&meshes);
	UnloadUnneeded(&materials);
	UnloadUnneeded(&textures);
	UnloadUnneeded(&shaders);
}

bool AssetManager::GetRenderable(Renderable* renderable, std::string materialName, std::string meshName)
{
	if (materials.count(materialName) == 0)
	{
		std::cout << "Failed to find material " << materialName << "\n";
		return false;
	}
	if (materials[materialName].isLoaded == false)
	{
		std::cout << "Material " << materialName << " is not loaded.\n";
		if (!LoadMaterial(materialName)) { return false; }
	}
	if (meshes.count(meshName) == 0)
	{
		std::cout << "Failed to find mesh " << meshName << "\n";
		return false;
	}
	if (meshes[meshName].isLoaded == false)
	{
		std::cout << "Mesh " << meshName << " is not loaded.\n";
		if (!LoadMesh(meshName)) { return false; }
	}
	renderable->SetMaterial(&materials[materialName]);
	renderable->SetMesh(&meshes[meshName]);
	return true;
}

AssetManager::AssetManager()
{
}


AssetManager::~AssetManager()
{
	SetNotNeeded(&meshes);
	SetNotNeeded(&materials);
	SetNotNeeded(&textures);
	SetNotNeeded(&shaders);

	UnloadUnneeded(&meshes);
	UnloadUnneeded(&materials);
	UnloadUnneeded(&textures);
	UnloadUnneeded(&shaders);
}
