#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

#include "meMath.h"
#include "meMesh.h"
#include "meMaterial.h"
#include "meAsset.h"

class Renderable
{
	Asset* mesh;
	Asset* material;
public:
	void Render();
	Renderable(Asset* material, Asset* mesh);
	void SetMesh(Asset* mesh);
	void SetMaterial(Asset* material);
	~Renderable();
};

