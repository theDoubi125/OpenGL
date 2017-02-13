#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>

#include "shader.h"
#include "scene.h"
#include "entity.h"
#include "cube.h"

# define PI           3.14159265358979323846

Scene::Scene(vec2 screenSize) : m_screenSize(screenSize)
{
	Cube *cube = new Cube(this);
	m_entities.push_back(cube);
	m_projectionMatrix = glm::perspective(70.0, (double)(m_screenSize.x / m_screenSize.y), 0.001, 100.0);
}

Scene::Scene(const Scene &scene)
{

}

Scene::~Scene()
{
	for (int i = 0; i < m_entities.size(); i++)
		delete m_entities[i];
}

void Scene::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < m_entities.size(); i++)
	{
		m_entities[i]->init();
	}
}

void Scene::update(const float &deltaTime)
{
	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->update(deltaTime);
}

void Scene::render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->render();
	
	glutSwapBuffers();
}

void Scene::onResize(vec2 screenSize)
{
	m_screenSize = screenSize;
	m_projectionMatrix = glm::perspective(70.0, (double)(m_screenSize.x / m_screenSize.y), 0.001, 100.0);
}

vec2 Scene::screenSize() const
{
	return m_screenSize;
}

const mat4& Scene::projectionMatrix() const
{
	return m_projectionMatrix;
}