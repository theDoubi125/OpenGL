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

StateMachine::StateMachine(Entity* entity) : Component(entity)
{
	//pushState(initState);
}

StateMachine::~StateMachine()
{
	while (!m_states.empty())
	{
		delete m_states.top();
		m_states.pop();
	}
}

void StateMachine::init(json descr)
{
	pushState(new IdleBehaviour());
}

void StateMachine::start()
{

}

void StateMachine::render() const
{

}

Component* StateMachine::createInstance(Entity* entity) const
{
	return new StateMachine(entity);
}

void StateMachine::pushState(CubeBehaviour *state)
{
	state->setStateMachine(this);
	m_states.push(state);
	state->enter();
}

void StateMachine::popState()
{
	if (m_states.empty())
		throw std::exception("Empty state machine stack");
	delete m_states.top();
	m_states.pop();
}

void StateMachine::replaceState(CubeBehaviour *state)
{
	popState();
	pushState(state);
}

std::string StateMachine::name() const
{
	return "StateMachine";
}

Transform& StateMachine::transform()
{
	return m_entity->transform();
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
	m_initialPos = transform.getPosition();
	vec3 dir = normalize(-vec3(m_dir.x, 0, m_dir.y));
	dir /= 2;
	dir.y = 0.5;
	m_rotCenter = inverse(transform.getRotation()) * dir;
}

void RollingBehaviour::update(float deltaTime)
{
	deltaTime = min(deltaTime, m_stepDuration-m_time);
	//m_angle = (float)(m_time * PI / 2 / m_stepDuration);
	glm::vec3 moveDir = vec3(m_dir.x, 0, m_dir.y);
	Transform &transform = m_machine->transform();
	//transform.setPosition(m_initalPos + vec3(-cos(m_angle + PI/4)/2 + cos(PI/4)/2, sin(m_angle + PI / 4)/2- sin(PI / 4) / 2, 0));
	glm::vec3 rotDir = glm::cross(glm::vec3(0, 1, 0), moveDir);
	glm::vec3 pos = glm::inverse(transform.rotationMatrix()) * glm::vec4(rotDir.x, rotDir.y, rotDir.z, 1);
	transform.rotate(m_rotCenter, pos, deltaTime * PI / 2 / m_stepDuration);
	//transform.setPosition(m_rotCenterWorldPos + transform.rotationMatrix() * m_rotCenterLocalPos);
	if (m_time + deltaTime >= m_stepDuration)
		m_machine->popState();
	m_time += deltaTime;
}
