#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "entity.h"
#include "shader.h"

Transform::Transform() : m_localRot(), m_localPos(0, 0, 0), m_localScale(1, 1, 1), m_parent(NULL)
{

}

Transform::Transform(const vec3 &pos, const quat &rot, const vec3 &scale) : m_localRot(rot), m_localPos(pos), m_localScale(scale), m_parent(NULL)
{

}

Transform::Transform(Transform *parent, const vec3 &localPos, const quat &localRot, const vec3 &scale) : m_localRot(localRot), m_localPos(localPos), m_localScale(scale), m_parent(parent)
{

}

Transform::Transform(const Transform &transform) : m_localPos(transform.m_localPos), m_localRot(transform.m_localRot), m_localScale(transform.m_localScale)
{

}

Transform::~Transform()
{

}

Transform* const Transform::getParent() const
{
	return m_parent;
}

void Transform::setParent(Transform *parent)
{
	m_parent = parent;
}

mat4x4 Transform::getGlobalMatrix() const
{
	if (m_parent == NULL)
		return getLocalMatrix();
	return getLocalMatrix() * m_parent->getGlobalMatrix();
}

mat4x4 Transform::getLocalMatrix() const
{
	mat4 t = glm::translate(mat4(1.0f), m_localPos);
	mat4 r = glm::mat4_cast(m_localRot);
	mat4 s = glm::scale(mat4(1.0f), m_localScale);
	return t * r * s;
}

const vec3& Transform::getPosition() const
{
	return m_localPos;
}

const vec3& Transform::getScale() const
{
	return m_localScale;
}

const quat& Transform::getRotation() const
{
	return m_localRot;
}

void Transform::rotate(const quat &rot)
{
	m_localRot *= rot;
}

void Transform::rotate(const vec3 &axis, float angle)
{
	m_localRot *= angleAxis(angle, axis);
}

void Transform::translate(const vec3& v)
{
	m_localPos += v;
}

void Transform::scale(const vec3 &v)
{
	m_localScale *= v;
}

Entity::Entity(const Scene *scene, Shader* shader) : m_scene(scene), m_mesh(NULL), m_shader(shader)
{

}

Entity::Entity(const Entity& entity) : m_transform(entity.m_transform), m_scene(entity.m_scene), m_mesh(entity.m_mesh)
{

}

Entity::~Entity()
{
	delete m_shader;
}

void Entity::init()
{
	m_modelMatrixId = glGetUniformLocation(m_shader->getProgramId(), "modelMatrix");
	m_viewMatrixId = glGetUniformLocation(m_shader->getProgramId(), "viewMatrix");
	m_projMatrixId = glGetUniformLocation(m_shader->getProgramId(), "projectionMatrix");
	m_shader->load();
	m_mesh->init();
}

void Entity::render() const
{
	if (m_shader == NULL)
		return;

	mat4 transformMatrix = transform().getGlobalMatrix();
	glUseProgram(m_shader->getProgramId());
	std::cout << glm::to_string(scene().projectionMatrix() * transform().getGlobalMatrix() * glm::vec4(0.0, 0.0, -5.0, 1.0)) << std::endl;
	glUniformMatrix4fv(m_modelMatrixId, 1, GL_FALSE, glm::value_ptr(mat4(1)));
	glUniformMatrix4fv(m_viewMatrixId, 1, GL_FALSE, glm::value_ptr(mat4(1)));
	glUniformMatrix4fv(m_projMatrixId, 1, GL_FALSE, glm::value_ptr(mat4(1)));
	m_mesh->render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Entity::update(const float &deltaTime)
{

}

Transform& Entity::transform()
{
	return m_transform;
}

const Transform& Entity::transform() const
{
	return m_transform;
}

bool Entity::isAlive() const
{
	return true;
}

const Scene& Entity::scene() const
{
	return *m_scene;
}

void Entity::setMesh(Mesh* mesh)
{
	m_mesh = mesh;
}