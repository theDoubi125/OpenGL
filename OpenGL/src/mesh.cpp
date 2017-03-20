#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SOIL.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>

#include "shader.h"
#include "mesh.h"
#include "entity.h"

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif
std::map<std::string, Mesh*> Mesh::m_registeredMeshes;

Mesh::Mesh() : m_vboId(0)
{

}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vboId);
}

void Mesh::init(json descr)
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

void Mesh::registerMesh(std::string name, Mesh* mesh)
{
	m_registeredMeshes[name] = mesh;
}

void Mesh::registerMeshes()
{
	registerMesh("Cube", new CubeMesh());
}

Mesh* Mesh::getRegisteredMesh(std::string name)
{
	return m_registeredMeshes[name];
}

CubeMesh::CubeMesh()
{

}

CubeMesh::~CubeMesh()
{
	delete[] m_vertices;
	delete[] m_textCoords;
}

void CubeMesh::init(json descr)
{
	m_vertices = new float[72]
	{
		-0.5, -0.5, -0.5,	-0.5, 0.5, -0.5,	0.5, 0.5, -0.5,		0.5, -0.5, -0.5,
		-0.5, -0.5, 0.5,	0.5, -0.5, 0.5,		0.5, 0.5, 0.5,		-0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,	0.5, -0.5, -0.5,	0.5, -0.5, 0.5,		-0.5, -0.5, 0.5,
		-0.5, 0.5, -0.5,	-0.5, 0.5, 0.5,		0.5, 0.5, 0.5,		0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,	-0.5, -0.5, 0.5,	-0.5, 0.5, 0.5,		-0.5, 0.5, -0.5,
		0.5, -0.5, -0.5,	0.5, 0.5, -0.5,		0.5, 0.5, 0.5,		0.5, -0.5, 0.5
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

	Mesh::init(descr);
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

MeshRenderer::MeshRenderer(Entity* entity) : Component(entity), m_shader(NULL), m_textureParams(NULL), m_textures(NULL), m_textureCount(0)
{

}

MeshRenderer::~MeshRenderer()
{
	delete[] m_textures;
	delete[] m_textureParams;
}

void MeshRenderer::init(json descr)
{
	json shaderDescr = descr["Shader"];
	m_shader = new Shader(shaderDescr["Vertex"], shaderDescr["Fragment"]);
	m_shader->load();
	m_modelMatrixId = glGetUniformLocation(m_shader->getProgramId(), "modelMatrix");
	m_viewMatrixId = glGetUniformLocation(m_shader->getProgramId(), "viewMatrix");
	m_projMatrixId = glGetUniformLocation(m_shader->getProgramId(), "projectionMatrix");
	m_mesh = Mesh::getRegisteredMesh(descr["Mesh"]);
	m_mesh->init(descr);

	m_texture = SOIL_load_OGL_texture
	(
		"resources/img/box.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	m_texture2 = SOIL_load_OGL_texture
	(
		"resources/img/rock.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	json texturesDescr = shaderDescr["Textures"];
	m_textureCount = texturesDescr.size();
	m_textureParams = new std::string[m_textureCount];
	m_textures = new GLuint[m_textureCount];

	for (int i = 0; i < m_textureCount; i++)
	{
		json textureDescr = texturesDescr[i];
		const std::string param = textureDescr["Param"];
		const std::string path = textureDescr["Path"];
		std::cout << param << std::endl;
		m_textureParams[i] = param;
		m_textures[i] = SOIL_load_OGL_texture
		(
			path.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	}
}

void MeshRenderer::start()
{

}

void MeshRenderer::update(float deltaTime)
{

}

void MeshRenderer::render() const
{
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_shader->getProgramId());
	glUniformMatrix4fv(m_modelMatrixId, 1, GL_FALSE, glm::value_ptr(m_entity->transform().getGlobalMatrix()));
	glm::mat4 viewMatrix = m_entity->scene().viewMatrix();
	glUniformMatrix4fv(m_viewMatrixId, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(m_projMatrixId, 1, GL_FALSE, glm::value_ptr(m_entity->scene().projectionMatrix()));

	for (int i = 0; i < m_textureCount; i++)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glUniform1i(glGetUniformLocation(m_shader->getProgramId(), m_textureParams[i].c_str()), i);
	}
	//glUniform1i(glGetUniformLocation(m_shader->getProgramId(), "text2"), 1);
	
	for (int i = 0; i < m_textureCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
	}

	m_mesh->render();
}

Component* MeshRenderer::createInstance(Entity* entity) const
{
	return new MeshRenderer(entity);
}
