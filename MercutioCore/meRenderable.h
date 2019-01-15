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
	Material* material;
public:
	void Render();
	Renderable(Mesh* mesh, Material* material);
	~Renderable();
};

