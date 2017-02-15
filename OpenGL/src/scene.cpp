#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>

#include "shader.h"
#include "scene.h"
#include "entity.h"
#include "cube.h"

# define PI           3.14159265358979323846

Scene::Scene(vec2 screenSize) : m_screenSize(screenSize), m_camera(new Camera(70, (double)(screenSize.x/screenSize.y), 0.001, 100))
{
	m_camera->transform().translate(vec3(0, 2, 0));
	m_test = new Cube(this);
	m_test->transform().translate(vec3(0, -1, -5));
	m_test->transform().rotate(vec3(0, 1, 0), 50);
	m_entities.push_back(m_test);
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

	m_test->transform().rotate(vec3(0, 1, 0), deltaTime);
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
	m_camera->setScreenRatio(screenSize.x / screenSize.y);
}

vec2 Scene::screenSize() const
{
	return m_screenSize;
}

const mat4& Scene::projectionMatrix() const
{
	return m_camera->projectionMatrix();
}

const mat4& Scene::viewMatrix() const
{
	return m_camera->viewMatrix();
}

Camera::Camera(float fov, float ratio, float nearDist, float farDist) : m_fov(fov), m_ratio(ratio), m_near(nearDist), m_far(far), m_transform(new Transform())
{
	m_projectionMatrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
}

Camera::~Camera()
{
	delete m_transform;
}

const glm::mat4& Camera::projectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::mat4& Camera::viewMatrix() const
{
	return glm::transpose(m_transform->getGlobalMatrix());
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