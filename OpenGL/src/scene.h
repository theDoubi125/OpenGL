#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <vector>
#include <glm/vec2.hpp>

class Shader;
class GameEntity
{
public:
	virtual void init() = 0;
	virtual void update(const float &deltaTime) = 0;
	virtual void render() const = 0;
};

class Entity;
class Scene : public GameEntity
{
public:
	Scene(glm::vec2 screenSize);
	Scene(const Scene &scene);
	~Scene();

	virtual void update(const float &deltaTime);
	virtual void render() const;
	virtual void init();
	void onResize(glm::vec2 screenSize);

	glm::vec2 screenSize() const;
	const glm::mat4& projectionMatrix() const;

private:
	std::vector<Entity*> m_entities;
	glm::vec2 m_screenSize;
	glm::mat4 m_projectionMatrix;

	Entity* m_test;
};

#endif SCENE_HEADER
