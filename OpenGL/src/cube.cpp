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
#include "input.h"

Cube::Cube(const Scene* scene) : Entity(scene), m_time(0), m_behaviour(NULL)
{
	
}

Cube::Cube(const Cube &cube) : Entity(cube), m_time(0), m_behaviour(NULL)
{

}

Cube::~Cube()
{
	delete m_behaviour;
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
	m_behaviour = new IdleBehaviour(*this);
}

void Cube::render() const
{
	mat4 transformMatrix = transform().getGlobalMatrix();
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	Entity::render();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cube::update(float deltaTime)
{
	m_time += deltaTime;
	m_behaviour->update(deltaTime);
}

void Cube::setBehaviour(CubeBehaviour *behaviour)
{
	if (m_behaviour != NULL)
		delete m_behaviour;
	m_behaviour = behaviour;
}

CubeBehaviour::CubeBehaviour(Cube &cube) : m_cube(cube)
{

}

CubeBehaviour::~CubeBehaviour()
{

}

IdleBehaviour::IdleBehaviour(Cube& cube) : CubeBehaviour(cube)
{

}

IdleBehaviour::~IdleBehaviour()
{

}

void IdleBehaviour::update(float deltaTime)
{
	if (Input::instance.isKeyDown(GLUT_KEY_RIGHT))
	{
		m_cube.setBehaviour(new RollingBehaviour(m_cube, glm::ivec2(1, 0)));
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_LEFT))
	{
		m_cube.setBehaviour(new RollingBehaviour(m_cube, glm::ivec2(-1, 0)));
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_UP))
	{
		m_cube.setBehaviour(new RollingBehaviour(m_cube, glm::ivec2(0, -1)));
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_DOWN))
	{
		m_cube.setBehaviour(new RollingBehaviour(m_cube, glm::ivec2(0, 1)));
	}
}

RollingBehaviour::RollingBehaviour(Cube& cube, glm::ivec2 dir) : CubeBehaviour(cube), m_dir(dir)
{

}

RollingBehaviour::~RollingBehaviour()
{

}

void RollingBehaviour::update(float deltaTime)
{
	m_cube.transform().translate(glm::vec3(m_dir.x, 0, m_dir.y) * deltaTime);
}