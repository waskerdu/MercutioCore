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
	GLuint shaderProgram;

public:
	void UseMaterial();
	Material(GLuint texture, GLuint shaderProgram);
	~Material();
};

