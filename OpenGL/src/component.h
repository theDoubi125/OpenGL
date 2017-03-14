#ifndef COMPONENT_INCLUDED
#define COMPONENT_INCLUDED

#include "json.hpp"

using json = nlohmann::json;

class Entity;

class Component
{
public:
	Component(Entity* entity);
	virtual ~Component();
	virtual void init(json descr) = 0;
	virtual void start() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() const = 0;
	virtual Component* createInstance(Entity* entity) const = 0;

	static Component* createComponent(std::string name, Entity* entity);

	static void registerComponents();

protected:
	Entity* m_entity;

	static void registerComponent(std::string name, Component* model);

private:
	static std::map<std::string, Component*> m_componentModels;
};

#endif COMPONENT_INCLUDED