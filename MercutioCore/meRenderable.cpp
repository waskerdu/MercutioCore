#include "stdafx.h"
#include "meRenderable.h"

void Renderable::Render()
{
	return;
}

Renderable::Renderable(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;
}


Renderable::~Renderable()
{
}
