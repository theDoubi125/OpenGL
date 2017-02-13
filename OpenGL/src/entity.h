#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include "scene.h"
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\quaternion.hpp>
#include <iostream>

#include "mesh.h"

using namespace glm;

class Transform
{
public:
	Transform();
	Transform(const vec3 &pos, const quat &rot, const vec3 &scale);
	Transform(Transform *parent, const vec3 &localPos, const quat &localRot, const vec3 &scale);
	Transform(const Transform &transform);

	virtual ~Transform();

	Transform* const getParent() const;
	void setParent(Transform *parent);

	mat4x4 getLocalMatrix() const;

	mat4x4 getGlobalMatrix() const;

	const vec3& getPosition() const;
	const vec3& getScale() const;
	const quat& getRotation() const;
	
	void rotate(const quat &rot);
	void rotate(const vec3 &axis, float angle);
	void translate(const vec3 &v);
	void scale(const vec3 &v);

private:
	vec3 m_localPos;
	vec3 m_localScale;
	quat m_localRot;
	Transform *m_parent;
};

class Entity : public GameEntity
{
public:
	Entity(const Scene *scene);
	Entity(const Entity& model);
	virtual ~Entity();

	virtual void init();
	virtual void update(const float &deltaTime);
	virtual void render() const;

	Transform& transform();
	const Transform& transform() const;

	virtual bool isAlive() const;

	void setMesh(Mesh *mesh);

protected:
	const Scene& scene() const;

private:
	Transform m_transform;
	const Scene* m_scene;
	Shader* m_shader;
	Mesh* m_mesh;
	GLuint m_modelMatrixId, m_viewMatrixId, m_projMatrixId;
};

#endif
