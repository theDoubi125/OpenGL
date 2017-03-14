#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <vector>
#include <glm/vec2.hpp>
#include <json.hpp>

using json = nlohmann::json;

class World;
class Shader;
class GameEntity
{
public:
	virtual void init(json descr) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() const = 0;
};

class Camera;
class Entity;
class Scene : public GameEntity
{
public:
	Scene(glm::vec2 screenSize);
	Scene(const Scene &scene);
	~Scene();

	virtual void update(float deltaTime);
	virtual void render() const;
	virtual void init(json descr);
	void onResize(glm::vec2 screenSize);

	glm::vec2 screenSize() const;
	const glm::mat4 projectionMatrix() const;
	const glm::mat4 viewMatrix() const;

private:
	std::vector<Entity*> m_entities;
	glm::vec2 m_screenSize;

	Entity* m_test;
	Camera* m_camera;
	float m_time;
	World *m_world;
};

class Transform;
class Camera
{
public:
	Camera(float fov, float ratio, float nearDist, float farDist);
	~Camera();

	const glm::mat4 projectionMatrix() const;
	const glm::mat4 viewMatrix() const;

	void setScreenRatio(float ratio);
	Transform& transform();

private:
	Transform *m_transform;
	glm::mat4 m_projectionMatrix;
	double m_fov;
	double m_near, m_far, m_ratio;
};

#endif SCENE_HEADER
