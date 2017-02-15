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

Cube::Cube(const Scene* scene) : Entity(scene), m_time(0), m_stateMachine(NULL)
{

}

Cube::Cube(const Cube &cube) : Entity(cube), m_time(0), m_stateMachine(NULL)
{

}

Cube::~Cube()
{
	delete m_stateMachine;
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
	m_stateMachine = new StateMachine(*this, new IdleBehaviour(*this));
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
	m_stateMachine->update(deltaTime);
}

void Cube::setBehaviour(CubeBehaviour *behaviour)
{
	m_stateMachine->pushState(behaviour);
}

StateMachine::StateMachine(Cube &cube, CubeBehaviour *initState) : m_cube(cube)
{
	m_states.push(initState);
}

StateMachine::~StateMachine()
{
	while (!m_states.empty())
	{
		delete m_states.top();
		m_states.pop();
	}
}

void StateMachine::pushState(CubeBehaviour *state)
{
	m_states.push(state);
}

void StateMachine::popState()
{
	delete m_states.top();
	m_states.pop();
}

void StateMachine::replaceState(CubeBehaviour *state)
{
	popState();
	pushState(state);
}

CubeBehaviour& StateMachine::currentState() const
{
	return *m_states.top();
}

void StateMachine::update(float deltaTime)
{
	m_states.top()->update(deltaTime);
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
	ivec2 dir = vec2(0);
	if (Input::instance.isKeyDown(GLUT_KEY_RIGHT))
	{
		dir += vec2(1, 0);
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_LEFT))
	{
		dir += vec2(-1, 0);
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_UP))
	{
		dir += vec2(0, -1);
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_DOWN))
	{
		dir += vec2(0, 1);
	}
	m_cube.transform().translate(glm::vec3(m_dir.x, 0, m_dir.y) * deltaTime);
	if (m_dir != dir)
		m_cube.setBehaviour(new IdleBehaviour(m_cube));
}
