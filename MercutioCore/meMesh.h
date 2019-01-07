#pragma once
#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

class Mesh
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;
	friend class AssetManager;
public:
	unsigned int GetVAO();
	unsigned int GetEBO();
	unsigned int GetVBO();
public:
	void Init();
	void Bind();
	int GetSize();

	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
	~Mesh();
};

