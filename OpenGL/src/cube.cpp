#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "mesh.h"
#include "entity.h"
#include "shader.h"
#include "cube.h"
#include <glm/gtx/string_cast.hpp>
#include <SOIL.h>

Cube::Cube(const Scene* scene) : Entity(scene, new Shader("resources/shaders/shader_base.vert", "resources/shaders/shader_base.frag")), m_time(0)
{
	
}

Cube::Cube(const Cube &cube) : Entity(cube), m_time(0)
{

}

Cube::~Cube()
{
}

void Cube::init()
{
	setMesh(new CubeMesh());
	m_texture = SOIL_load_OGL_texture
	(
		"G:/Documents/Julien/Projets/C++/OpenGL/OpenGL/resources/img/box.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (0 == m_texture)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	m_vertices = new float[72]
	{
		-0.5, -0.5, -0.5,  0.5, -0.5, -0.5,  0.5, 0.5, -0.5,  -0.5, 0.5, -0.5,
		-0.5, -0.5, 0.5,  0.5, -0.5, 0.5,  0.5, 0.5, 0.5,  -0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,  0.5, -0.5, -0.5,  0.5, -0.5, 0.5,  -0.5, -0.5, 0.5,
		-0.5, 0.5, -0.5,  0.5, 0.5, -0.5,  0.5, 0.5, 0.5,  -0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,  -0.5, -0.5, 0.5,  -0.5, 0.5, 0.5,  -0.5, 0.5, -0.5,
		0.5, -0.5, -0.5,  0.5, -0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, -0.5,
	};
	m_texCoords = new float[48]
	{
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
	};
	transform().translate(vec3(0, 0, -5));
	Entity::init();
}

void Cube::render() const
{
	if (m_shader == NULL)
		return;

	glUseProgram(m_shader->getProgramId());
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniformMatrix4fv(m_modelMatrixId, 1, GL_FALSE, glm::value_ptr(transform().getGlobalMatrix()));
	glUniformMatrix4fv(m_viewMatrixId, 1, GL_FALSE, glm::value_ptr(mat4(1)));
	glUniformMatrix4fv(m_projMatrixId, 1, GL_FALSE, glm::value_ptr(scene().projectionMatrix()));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, m_texCoords);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	/*
	glBindTexture(GL_TEXTURE_2D, m_texture);
	Entity::render();
	glBindTexture(GL_TEXTURE_2D, 0);*/
}

void Cube::update(const float &deltaTime)
{
	m_time += deltaTime;
	transform().rotate(vec3(0, 1, 0.5), deltaTime);
	
}