#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "entity.h"
#include "shader.h"
#include "cube.h"
#include <glm/gtx/string_cast.hpp>
#include <SOIL.h>

Cube::Cube(const Scene* scene) : Entity(scene), m_shader(new Shader("resources/shaders/shader_base.vert", "resources/shaders/shader_base.frag")), m_time(0), m_vertices(NULL)
{
	
}

Cube::Cube(const Cube &cube) : Entity(cube), m_shader(new Shader(*cube.m_shader)), m_time(0), m_vertices(NULL)
{

}

Cube::~Cube()
{
	delete m_shader;
}

void Cube::init()
{
	setMesh(new CubeMesh());
	Entity::init();
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
	m_texture = SOIL_load_OGL_texture
	(
		"resources/img/box.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (0 == m_texture)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	m_shader->load();
	m_modelMatrixId = glGetUniformLocation(m_shader->getProgramId(), "modelMatrix");
	m_viewMatrixId = glGetUniformLocation(m_shader->getProgramId(), "viewMatrix");
	m_projMatrixId = glGetUniformLocation(m_shader->getProgramId(), "projectionMatrix");
	transform().translate(vec3(0, 0, -5));
}

void Cube::render() const
{
	if (m_shader == NULL)
		return;
	
	mat4 transformMatrix = transform().getGlobalMatrix();
	glUseProgram(m_shader->getProgramId());
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniformMatrix4fv(m_modelMatrixId, 1, GL_FALSE, glm::value_ptr(transform().getGlobalMatrix()));
	glUniformMatrix4fv(m_viewMatrixId, 1, GL_FALSE, glm::value_ptr(mat4(1)));
	glUniformMatrix4fv(m_projMatrixId, 1, GL_FALSE, glm::value_ptr(scene().projectionMatrix()));
	
	Entity::render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Cube::update(const float &deltaTime)
{
	m_time += deltaTime;
	transform().rotate(vec3(0, 1, 0.5), deltaTime);
	
}