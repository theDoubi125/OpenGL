#include "entity.h"
#include <glm\gtc\matrix_transform.hpp>

Transform::Transform() : m_localRot(), m_localPos(0, 0, 0), m_localScale(1, 1, 1), m_parent(NULL)
{

}

Transform::Transform(const vec3 &pos, const quat &rot, const vec3 &scale) : m_localRot(rot), m_localPos(pos), m_localScale(scale), m_parent(NULL)
{

}

Transform::Transform(Transform *parent, const vec3 &localPos, const quat &localRot, const vec3 &scale) : m_localRot(localRot), m_localPos(localPos), m_localScale(scale), m_parent(parent)
{

}

Transform::~Transform()
{

}

vec3& Transform::localPos()
{
	return m_localPos;
}

const vec3& Transform::localPos() const
{
	return m_localPos;
}

vec3& Transform::localScale()
{
	return m_localScale;
}

const vec3& Transform::localScale() const
{
	return m_localScale;
}

quat Transform::localRot()
{
	return m_localRot;
}

const quat& Transform::localRot() const
{
	return m_localRot;
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
	mat4 t = translate(mat4(1.0f), m_localPos);
	mat4 r = mat4_cast(m_localRot);
	mat4 s = scale(mat4(1.0f), m_localScale);
	return t * s * r;
}