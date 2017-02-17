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
	m_stateMachine = new StateMachine(*this, new IdleBehaviour());
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
	pushState(initState);
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
	state->setStateMachine(this);
	m_states.push(state);
}

void StateMachine::popState()
{
	if (m_states.empty())
		throw std::exception("Empty stateMachine stack");
	delete m_states.top();
	m_states.pop();
}

void StateMachine::replaceState(CubeBehaviour *state)
{
	popState();
	pushState(state);
}

Transform& StateMachine::transform()
{
	return m_cube.transform();
}

CubeBehaviour& StateMachine::currentState() const
{
	return *m_states.top();
}

void StateMachine::update(float deltaTime)
{
	m_states.top()->update(deltaTime);
}

CubeBehaviour::CubeBehaviour() : m_machine(NULL)
{

}

CubeBehaviour::~CubeBehaviour()
{

}

void CubeBehaviour::setStateMachine(StateMachine *machine)
{
	m_machine = machine;
}

IdleBehaviour::IdleBehaviour()
{

}

IdleBehaviour::~IdleBehaviour()
{

}

void IdleBehaviour::update(float deltaTime)
{
	if (Input::instance.isKeyDown(GLUT_KEY_RIGHT))
	{
		m_machine->pushState(new RollingBehaviour(glm::ivec2(1, 0)));
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_LEFT))
	{
		m_machine->pushState(new RollingBehaviour(glm::ivec2(-1, 0)));
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_UP))
	{
		m_machine->pushState(new RollingBehaviour(glm::ivec2(0, -1)));
	}
	else if (Input::instance.isKeyDown(GLUT_KEY_DOWN))
	{
		m_machine->pushState(new RollingBehaviour(glm::ivec2(0, 1)));
	}
}

void IdleBehaviour::enter()
{

}

RollingBehaviour::RollingBehaviour(glm::ivec2 dir) : m_dir(dir), m_stepDuration(1), m_stepDist(1), m_time(0)
{

}

RollingBehaviour::~RollingBehaviour()
{

}

void RollingBehaviour::enter()
{
	Transform &transform = m_machine->transform();
	m_rotCenterLocalPos = glm::inverse(transform.rotationMatrix()) * glm::vec4(0.5, -0.5, 0, 1);
	vec3 rotCenter = transform.getPosition() + glm::vec3(0.5, -0.5, 0);
	m_rotCenterWorldPos = vec4(rotCenter.x, rotCenter.y, rotCenter.z, 1);
}

void RollingBehaviour::update(float deltaTime)
{
	deltaTime = min(deltaTime, m_stepDuration-m_time);
	glm::vec3 moveDir = glm::vec3(m_dir.x, 0, m_dir.y);
	Transform &transform = m_machine->transform();
	transform.translate(moveDir * deltaTime / m_stepDuration * m_stepDist);
	glm::vec3 rotDir = glm::cross(glm::vec3(0, 1, 0), moveDir);
	glm::vec3 pos = glm::inverse(transform.rotationMatrix()) * glm::vec4(rotDir.x, rotDir.y, rotDir.z, 1);
	transform.rotate(pos, deltaTime * PI / 2 / m_stepDuration);
	//transform.setPosition(m_rotCenterWorldPos + transform.rotationMatrix() * m_rotCenterLocalPos);
	if (m_time + deltaTime >= m_stepDuration)
		m_machine->popState();
	m_time += deltaTime;
}
