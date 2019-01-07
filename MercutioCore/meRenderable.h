#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

#include "meMath.h"
#include "meMesh.h"
#include "meMaterial.h"

class Renderable
{
	Mesh* mesh;
	GLuint* shader;
	GLuint texture;
public:
	Renderable();
	~Renderable();
};

