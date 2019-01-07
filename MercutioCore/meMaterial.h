#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include "preamble.glsl"

class Material
{
	GLuint texture;
	unsigned char* textureData;
	GLuint* shaderProgram;

public:
	void Init(GLuint texture, GLuint* shaderProgram);
	void UseMaterial();

	Material();
	~Material();
};

