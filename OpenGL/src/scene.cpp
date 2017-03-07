#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "scene.h"
#include "entity.h"
#include "cube.h"
#include "world.h"

Scene::Scene(vec2 screenSize) : m_time(0), m_screenSize(screenSize), m_camera(new Camera(70, (float)(screenSize.x/screenSize.y), 1, 100))
{
	m_test = new Cube(this);
	m_test->transform().translate(vec3(0, -1, -5));
	m_entities.push_back(m_test);
	World* world = new World(ivec3(5, 5, 5));
	world->setCell(ivec3(0, 0, 0), CELL_WALL);
	world->setCell(ivec3(0, 4, 0), CELL_WALL);
	world->setCell(ivec3(0, 1, 2), CELL_WALL);
}

Scene::Scene(const Scene &scene)
{
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
	m_camera->transform().setPosition(vec3(0, 1, 0));
}

void Scene::render() const
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->render();
	
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
