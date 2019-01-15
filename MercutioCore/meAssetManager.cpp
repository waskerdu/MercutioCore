#include "stdafx.h"
#include "meAssetManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.h"

void AssetManager::RegisterAssets()
{
	//
	std::vector<float> verts
	{
		//positions             //colors			    //texture coords
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
	//Asset* quadAsset = new Asset();
	//quadAsset->data = quadMesh;
	Asset quadAsset = Asset();
	quadAsset.data = quadMesh;
	quadAsset.isLoaded = true;
	quadAsset.isNeeded = true;
	meshes.insert(std::pair<std::string, Asset>("default", quadAsset));

	std::vector<std::vector<std::string>> data;
	manifest.GetSection("game_assets", &data);
	for (size_t i = 0; i < data.size(); i++)
	{
		Asset asset = Asset();
		asset.instructions = data[i];
		if (data[i][1] == "mesh")
		{
			meshes.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
		if (data[i][1] == "material")
		{
			materials.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
		if (data[i][1] == "renderable")
		{
			renderables.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
		if (data[i][1] == "texture")
		{
			textures.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
		if (data[i][1] == "shader")
		{
			shaders.insert(std::pair<std::string, Asset>(data[i][0], asset));
		}
	}
}

void AssetManager::SetNotNeeded(std::map<std::string, Asset>* map)
{
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
	std::map<std::string, Asset>::iterator i;
	for (i = map->begin(); i != map->end(); i++)
	{
		if (i->second.isNeeded == false)
		{
			std::string alias = i->second.name;
			map->at(alias).isLoaded = false;
			delete map->at(alias).data;
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
	std::vector<std::string>* instructions = &shaders[alias].instructions;
	//std::string alias = instructions->at(0);
	std::string filename = instructions->at(1);
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
		LoadTexture(instructions->at(2));
	}
	if (shaders.count(instructions->at(3)) == 0)
	{
		std::cout << "Failed to find shader " << instructions->at(3) << "\n";
		return false;
	}
	if (textures[instructions->at(3)].isLoaded == false)
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

bool AssetManager::LoadRenderable(std::string alias)
{
	if (renderables.count(alias) == 0)
	{
		std::cout << "No shader named " << alias << " exists.\n";
		return false;
	}
	if (renderables[alias].isLoaded)
	{
		return true;
	}
	std::vector<std::string>* instructions = &renderables[alias].instructions;

	if (meshes.count(instructions->at(2)) == 0)
	{
		std::cout << "Failed to find mesh " << instructions->at(2) << "\n";
		return false;
	}
	if (meshes[instructions->at(2)].isLoaded == false)
	{
		std::cout << "Mesh " << instructions->at(2) << " is not loaded.\n";
		//LoadMesh(instructions->at(3));
		return false;
	}
	if (materials.count(instructions->at(3)) == 0)
	{
		std::cout << "Failed to find material " << instructions->at(3) << "\n";
		return false;
	}
	if (materials[instructions->at(3)].isLoaded == false)
	{
		//std::cout << "Material " << instructions->at(3) << " is not loaded.\n";
		LoadMaterial(instructions->at(3));
	}
	Mesh* mesh = (Mesh*)meshes[instructions->at(2)].data;
	Material* material = (Material*)materials[instructions->at(3)].data;
	renderables[alias].data = new Renderable(mesh, material);
	renderables[alias].isLoaded = true;
	renderables[alias].isNeeded = true;
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
bool AssetManager::LoadPage(std::string name)
{
	//set all assets to "not needed"
	SetNotNeeded(&meshes);
	SetNotNeeded(&materials);
	SetNotNeeded(&textures);
	SetNotNeeded(&shaders);
	SetNotNeeded(&renderables);

	//for all assets in the page set them to needed and load them
	std::vector<std::vector<std::string>> data;
	manifest.GetSection(name, &data);

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i][0] == "meshes")
		{
			for (size_t f = 1; f < data[i].size(); f++)
			{

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
		if (data[i][0] == "renderables")
		{
			for (size_t f = 1; f < data[i].size(); f++)
			{
				LoadMaterial(data[i][f]);
			}
		}
	}

	//for all assets if they are not needed unload them
	UnloadUnneeded(&meshes);
	UnloadUnneeded(&materials);
	UnloadUnneeded(&textures);
	UnloadUnneeded(&shaders);
	UnloadUnneeded(&renderables);
	return true;
}

/*bool AssetManager::CreateShader(std::vector<std::string>* instructions)
{
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
	shaders.insert(std::pair<std::string, GLuint>(instructions->at(0), id));
	return true;
}

bool AssetManager::CreateTexture(std::vector<std::string>* instructions)
{
	GLuint id = 0;
	std::string alias = instructions->at(0);
	std::string filename = instructions->at(1);
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
	textures.insert(std::pair<std::string, GLuint>(alias, id));
	return true;
}

Mesh* AssetManager::GetMesh(std::string name)
{
	return nullptr;
}
Material* AssetManager::GetMaterial(std::string name)
{
	return nullptr;
}/**/
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
