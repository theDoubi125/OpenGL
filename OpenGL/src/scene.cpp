#include <GL\glew.h>
#include <GL\freeglut.h>

#include "shader.h"
#include "scene.h"
# define PI           3.14159265358979323846

Scene::Scene() : m_shader(new Shader("Shaders/shader_base.vert", "Shaders/shader_base.frag")), m_time(0)
{
	m_vertices = new float[9] { 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0 };
}

Scene::Scene(const Scene &scene) : m_shader(NULL), m_time(0)
{
	m_vertices = new float[9] { 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0 };
}

Scene::~Scene()
{
	delete m_shader;
	delete[] m_vertices;
}

void Scene::init()
{
	m_shader->load();
}

void Scene::update(const float &deltaTime)
{
	m_time += deltaTime;
	m_vertices[1] = cos(2 * m_time * PI);
	m_vertices[3] = cos(2 * m_time * PI);
}

void Scene::render() const
{
	if (m_shader == NULL)
		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
	glUseProgram(m_shader->getProgramId());
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
	glUseProgram(0);
}
