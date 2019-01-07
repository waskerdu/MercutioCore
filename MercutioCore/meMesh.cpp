#include "stdafx.h"
#include "meMesh.h"
#include "preamble.glsl"

unsigned int Mesh::GetVAO() { return VAO; }
unsigned int Mesh::GetEBO() { return EBO; }
unsigned int Mesh::GetVBO() { return VBO; }

void Mesh::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
	//position
	glVertexAttribPointer(POSITION_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(POSITION_ATTRIB_LOCATION);
	//color
	glVertexAttribPointer(COLOR_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(COLOR_ATTRIB_LOCATION);
	//text coord
	glVertexAttribPointer(TEXTURE_COORD_ATTRIB_LOCATION, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB_LOCATION);

	glBindVertexArray(0);
}

void Mesh::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetEBO());
	glBindBuffer(GL_ARRAY_BUFFER, GetVBO());
	glBindVertexArray(GetVAO());
}

int Mesh::GetSize() { return (int)vertices.size(); }

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Mesh::~Mesh()
{
}
