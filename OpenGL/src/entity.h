#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include "scene.h"
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\quaternion.hpp>
#include <iostream>

using namespace glm;

class Transform
{
public:
	Transform();
	Transform(const vec3 &pos, const quat &rot, const vec3 &scale);
	Transform(Transform *parent, const vec3 &localPos, const quat &localRot, const vec3 &scale);
	virtual ~Transform();

	vec3& localPos();
	const vec3& localPos() const;

	vec3& localScale();
	const vec3& localScale() const;

	quat localRot();
	const quat& localRot() const;

	Transform* const getParent() const;
	void setParent(Transform *parent);

	mat4x4 getLocalMatrix() const;

	mat4x4 getGlobalMatrix() const;

private:
	vec3 m_localPos;
	vec3 m_localScale;
	quat m_localRot;
	Transform *m_parent;
};

class Entity : public GameEntity
{
public:
	Entity();
	Entity(const Entity& model);
	virtual ~Entity();

	virtual void update(float deltaTime);
	virtual void render() const;

private:
	Transform m_transform;
};

#endif
