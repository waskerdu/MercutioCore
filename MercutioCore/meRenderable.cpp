#include "stdafx.h"
#include "meRenderable.h"

void Renderable::Render()
{
	return;
}

Renderable::Renderable(Asset* material, Asset* mesh)
{
	this->mesh = mesh;
	this->material = material;
}

void Renderable::SetMaterial(Asset* material)
{
	this->material = material;
}

void Renderable::SetMesh(Asset* mesh)
{
	this->mesh = mesh;
}

Renderable::~Renderable()
{
}
