#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>

#include "component.h"
#include "mesh.h"
#include "cube.h"

std::map<std::string, Component*> Component::m_componentModels;

Component::Component(Entity* entity) : m_entity(entity)
{

}

Component::~Component()
{

}

void Component::registerComponent(std::string name, Component* model)
{
	m_componentModels[name] = model;
}

Component* Component::createComponent(std::string name, Entity* entity)
{
	if (m_componentModels.count(name) > 0)
	{
		return m_componentModels[name]->createInstance(entity);
	}
	return NULL;
}

void Component::registerComponents()
{
	registerComponent("MeshRenderer", new MeshRenderer(NULL));
	registerComponent("StateMachine", new StateMachine(NULL));
}