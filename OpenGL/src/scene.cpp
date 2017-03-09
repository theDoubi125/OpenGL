#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "scene.h"
#include "entity.h"
#include "cube.h"
#include "world.h"

Scene::Scene(vec2 screenSize) : m_time(0), m_screenSize(screenSize), m_camera(new Camera(70, (float)(screenSize.x/screenSize.y), 1, 100)), m_world(NULL)
{
	m_test = new Cube(this);
	m_test->transform().translate(vec3(0, -1, -5));
	m_entities.push_back(m_test);
	m_test = new Cube(this);
	m_test->transform().translate(vec3(0, -1, -4));
	m_entities.push_back(m_test);
	m_world = new World(this, ivec3(5, 5, 5));
	m_world->transform().translate(vec3(0, 0, -4));
	m_entities.push_back(m_world);
	m_camera->transform().setPosition(vec3(-10, 0, 0));
}

Scene::Scene(const Scene &scene)
{
	delete m_world;
	delete m_camera;
}

Scene::~Scene()
{
	for (int i = 0; i < m_entities.size(); i++)
		delete m_entities[i];
}

void Scene::init()
{
	glEnable(GL_TEXTURE_2D);
	m_world->setCell(ivec3(1, 0, 1), CELL_WALL);
	for (int i = 1; i < 3; i++)
	{
		m_world->setCell(ivec3(i, 0, 0), CELL_WALL);
		m_world->setCell(ivec3(i, 1, 0), CELL_WALL);
	}
	for (int i = 0; i < m_entities.size(); i++)
	{
		m_entities[i]->init();
	}
}

void Scene::update(float deltaTime)
{
	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->update(deltaTime);

	m_time += deltaTime;
	m_camera->transform().setPosition(vec3(3, 2, 2));
	m_camera->transform().rotate(vec3(0, 1, 0), deltaTime);
}

void Scene::render() const
{
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < m_entities.size(); i++)
	{
		m_entities[i]->preRender();
		m_entities[i]->render();
		m_entities[i]->postRender();
	}
	
	glutSwapBuffers();
}

void Scene::onResize(vec2 screenSize)
{
	m_screenSize = screenSize;
	m_camera->setScreenRatio(screenSize.x / screenSize.y);
}

vec2 Scene::screenSize() const
{
	return m_screenSize;
}

const mat4 Scene::projectionMatrix() const
{
	return m_camera->projectionMatrix();
}

const mat4 Scene::viewMatrix() const
{
	return m_camera->viewMatrix();
}

Camera::Camera(float fov, float ratio, float nearDist, float farDist) : m_fov(fov), m_ratio(ratio), m_near(nearDist), m_far(farDist), m_transform(new Transform())
{
	m_projectionMatrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
}

Camera::~Camera()
{
	delete m_transform;
}

const glm::mat4 Camera::projectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::mat4 Camera::viewMatrix() const
{
	return m_transform->getReverseLocalMatrix();
}

void Camera::setScreenRatio(float ratio)
{
	m_ratio = ratio;
	m_projectionMatrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
}

Transform& Camera::transform()
{
	return *m_transform;
}
