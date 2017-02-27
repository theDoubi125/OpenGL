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
	return m_parent->getGlobalMatrix() * getLocalMatrix();
}

mat4x4 Transform::getReverseGlobalMatrix() const
{
	if (m_parent == NULL)
		return getReverseLocalMatrix();
	return getReverseLocalMatrix() * m_parent->getReverseLocalMatrix();
}

mat4x4 Transform::getLocalMatrix() const
{
	mat4 t = glm::translate(mat4(1.0f), m_localPos);
	mat4 r = glm::mat4_cast(m_localRot);
	mat4 s = glm::scale(mat4(1.0f), m_localScale);
	return t * r * s;
}

mat4x4 Transform::getReverseLocalMatrix() const
{
	mat4 t = glm::translate(mat4(1.0f), -m_localPos);
	mat4 r = glm::transpose(glm::mat4_cast(m_localRot));
	mat4 s = glm::scale(mat4(1.0f), vec3(1)/m_localScale);
	return s * r * t;
}

mat4 Transform::rotationMatrix() const
{
	return glm::mat4_cast(m_localRot);
}

mat4 Transform::reverseRotationMatrix() const
{

	return glm::transpose(glm::mat4_cast(m_localRot));
}

vec3 Transform::worldToLocal(vec3 pos) const
{
	return getReverseGlobalMatrix() * vec4(pos.x, pos.y, pos.z, 1);
}

vec3 Transform::localToWorld(vec3 pos) const
{
	return getGlobalMatrix() * vec4(pos.x, pos.y, pos.z, 1);
}

vec3 Transform::worldToLocalDir(vec3 pos) const
{
	return rotationMatrix() * vec4(pos.x, pos.y, pos.z, 1);
}

vec3 Transform::localToWorldDir(vec3 pos) const
{
	return rotationMatrix() * vec4(pos.x, pos.y, pos.z, 1);
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

void Transform::setPosition(const vec3& p)
{
	m_localPos = p;
}

void Transform::scale(const vec3 &v)
{
	m_localScale *= v;
}

Entity::Entity(const Scene *scene) : m_scene(scene), m_shader(new Shader("resources/shaders/shader_base.vert", "resources/shaders/shader_base.frag"))
{

}

Entity::Entity(const Entity& entity) : m_transform(entity.m_transform), m_scene(entity.m_scene), m_shader(new Shader(*entity.m_shader))
{

}

Entity::~Entity()
{
	delete m_shader;
}

void Entity::init()
{
	m_shader->load();
	m_modelMatrixId = glGetUniformLocation(m_shader->getProgramId(), "modelMatrix");
	m_viewMatrixId = glGetUniformLocation(m_shader->getProgramId(), "viewMatrix");
	m_projMatrixId = glGetUniformLocation(m_shader->getProgramId(), "projectionMatrix");
	m_mesh->init();
}

void Entity::render() const
{
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_shader->getProgramId());
	glUniformMatrix4fv(m_modelMatrixId, 1, GL_FALSE, glm::value_ptr(transform().getGlobalMatrix()));
	mat4 viewMatrix = scene().viewMatrix();
	glUniformMatrix4fv(m_viewMatrixId, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(m_projMatrixId, 1, GL_FALSE, glm::value_ptr(scene().projectionMatrix()));
	m_mesh->render();
	glUseProgram(0);
}

void Entity::update(float deltaTime)
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

void Entity::setMesh(Mesh *mesh)
{
	m_mesh = mesh;
}
