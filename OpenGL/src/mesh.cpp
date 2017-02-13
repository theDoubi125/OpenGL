#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "mesh.h"

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif

Mesh::Mesh() : m_vboId(0)
{

}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vboId);
}

void Mesh::init()
{
	if (glIsBuffer(m_vboId) == GL_TRUE)
		glDeleteBuffers(1, &m_vboId);
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

		int count = vertexCount();
		glBufferData(GL_ARRAY_BUFFER, count * (3 + 2) * sizeof(float), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * 3 * sizeof(float), getVertex());
		glBufferSubData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), count * 2 * sizeof(float), getTexCoords());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::render() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float) * 3 * vertexCount()));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_QUADS, 0, vertexCount());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

CubeMesh::CubeMesh()
{

}

CubeMesh::~CubeMesh()
{
	delete[] m_vertices;
	delete[] m_textCoords;
}

void CubeMesh::init()
{
	m_vertices = new float[72]
	{
		-0.5, -0.5, -0.5,  0.5, -0.5, -0.5,  0.5, 0.5, -0.5,  -0.5, 0.5, -0.5,
		-0.5, -0.5, 0.5,  0.5, -0.5, 0.5,  0.5, 0.5, 0.5,  -0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,  0.5, -0.5, -0.5,  0.5, -0.5, 0.5,  -0.5, -0.5, 0.5,
		-0.5, 0.5, -0.5,  0.5, 0.5, -0.5,  0.5, 0.5, 0.5,  -0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,  -0.5, -0.5, 0.5,  -0.5, 0.5, 0.5,  -0.5, 0.5, -0.5,
		0.5, -0.5, -0.5,  0.5, -0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, -0.5,
	};
	m_textCoords = new float[48]
	{
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
	};

	Mesh::init();
}

int CubeMesh::vertexCount() const
{
	return 24;
}

float* CubeMesh::getVertex() const
{
	return m_vertices;
}

float* CubeMesh::getTexCoords() const
{
	return m_textCoords;
}