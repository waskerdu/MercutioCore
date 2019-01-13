#include "stdafx.h"
#include "meAssetManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.h"

std::string AssetManager::GetExtension(std::string filename)
{
	size_t start = filename.find(".");
	if (start == std::string::npos) { return ""; }
	return filename.substr(start + 1);
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

void AssetManager::Init()
{
	manifest.Load("gamedata/asset_manifest.ini");
	LoadPage("init");
}
bool AssetManager::LoadPage(std::string name)
{
	std::vector<std::vector<std::string>> data;
	std::vector<std::vector<std::string>> meshEntries;
	std::vector<std::vector<std::string>> materialEntries;
	std::vector<std::vector<std::string>> renderableEntries;
	std::vector<std::vector<std::string>> shaderEntries;
	std::vector<std::vector<std::string>> textureEntries;

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
		else if (data[i][1] == "renderable")
		{
			//renderables.insert(std::pair<std::string, Renderable*>(data[i][0], nullptr));
			renderableEntries.push_back(data[i]);
		}
		else if (data[i][1] == "shader")
		{
			//renderables.insert(std::pair<std::string, Renderable*>(data[i][0], nullptr));
			shaderEntries.push_back(data[i]);
		}
		else if (data[i][1] == "texture")
		{
			//renderables.insert(std::pair<std::string, Renderable*>(data[i][0], nullptr));
			textureEntries.push_back(data[i]);
		}/**/
	}

	for (i = 0; i < textureEntries.size(); i++)
	{
		CreateTexture(&textureEntries.at(i));
	}
	for (i = 0; i < meshEntries.size(); i++)
	{
		//
	}
	for (i = 0; i < shaderEntries.size(); i++)
	{
		CreateShader(&shaderEntries.at(i));
	}
	for (i = 0; i < materialEntries.size(); i++)
	{
		//
	}/**/
	return true;
}

bool AssetManager::CreateShader(std::vector<std::string>* instructions)
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
