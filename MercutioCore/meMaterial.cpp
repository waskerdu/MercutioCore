#include "stdafx.h"
#include "meMaterial.h"

void Material::Init(GLuint texture, GLuint* shaderProgram)
{
	this->texture = texture;
	this->shaderProgram = shaderProgram;
	glBindTexture(GL_TEXTURE_2D, texture);

	glUseProgram(*shaderProgram);
	glUniform1i(TEXTURE_UNIFORM_LOCATION, 0);
}

void Material::UseMaterial()
{
	glUseProgram(*shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

Material::Material()
{
}

Material::~Material()
{
}
