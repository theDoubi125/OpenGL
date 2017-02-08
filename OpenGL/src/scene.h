#ifndef SCENE_HEADER
#define SCENE_HEADER

class Shader;
class GameEntity
{
public:
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() const = 0;
};

class Scene : public GameEntity
{
public:
	Scene();
	Scene(const Scene &scene);
	~Scene();

	virtual void update(const float deltaTime);
	virtual void render() const;
	virtual void init();

private:
	Shader* m_shader;
	float *m_vertices;
	float m_time;
};

#endif SCENE_HEADER