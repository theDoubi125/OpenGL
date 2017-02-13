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

Cube::Cube(const Scene* scene) : Entity(scene), m_time(0)
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
	Entity::init();
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
}

void Cube::render() const
{
	mat4 transformMatrix = transform().getGlobalMatrix();
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	Entity::render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Cube::update(const float &deltaTime)
{
	m_time += deltaTime;
}