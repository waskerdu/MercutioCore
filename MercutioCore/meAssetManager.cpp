#include "stdafx.h"
#include "meAssetManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
		}/**/
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
}

void AssetManager::CreateShader(std::vector<std::string>* instructions)
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
	}
	shaders.insert(std::pair<std::string, GLuint>(instructions->at(0), id));
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
