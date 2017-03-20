#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include "scene.h"
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\quaternion.hpp>
#include <iostream>
#include "json/json.h"

#include "mesh.h"
# define PI           3.14159265358979323846

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
	mat4x4 getReverseLocalMatrix() const;

	mat4x4 getGlobalMatrix() const;
	mat4x4 getReverseGlobalMatrix() const;

	mat4 rotationMatrix() const;
	mat4 reverseRotationMatrix() const;

	vec3 worldToLocal(vec3 pos) const;
	vec3 localToWorld(vec3 pos) const;

	// with rotation only
	vec3 worldToLocalDir(vec3 dir) const;
	vec3 localToWorldDir(vec3 dir) const;

	void setPosition(const vec3 &p);

	const vec3& getPosition() const;
	const vec3& getScale() const;
	const quat& getRotation() const;
	
	void rotate(const quat &rot);
	void rotate(const vec3 &axis, float angle);
	void rotate(const vec3 &center, const vec3 &axis, float angle);
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

	virtual void init(json descr);
	virtual void start();
	virtual void update(float deltaTime);
	virtual void render() const;

	void preRender() const;
	void postRender() const;

	Transform& transform();
	const Transform& transform() const;

	virtual bool isAlive() const;

	void addComponent(Component* component);

	Component* getComponent(std::string name) const;
	std::vector<Component*> getComponents(std::string name) const;

	const Scene& scene() const;

private:
	Transform m_transform;
	const Scene* m_scene;
	Shader* m_shader;
	GLuint m_modelMatrixId, m_viewMatrixId, m_projMatrixId;

	std::vector<Component*> m_components;
	std::map<std::string, std::vector<Component*> > m_componentsPerName;
};

#endif
