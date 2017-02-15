#ifndef CUBE_INCLUDED
#define CUBE_INCLUDED

#include <stack>
#include "entity.h"
class StateMachine;
class CubeBehaviour;
class Cube : public Entity
{
public:
	Cube(const Scene* scene);
	Cube(const Cube &model);
	~Cube();

	virtual void init() override;
	virtual void render() const override;
	virtual void update(float deltaTime) override;
	void setBehaviour(CubeBehaviour *behaviour);

private:
	float m_time;
	GLuint m_texture;
	glm::ivec2 m_movingDir;
	StateMachine *m_stateMachine;
};

class CubeBehaviour
{
public:
	CubeBehaviour(Cube &cube);
	virtual ~CubeBehaviour();
	virtual void update(float deltaTime) = 0;
protected:
	Cube &m_cube;
};

class StateMachine
{
public:
	StateMachine(Cube &cube, CubeBehaviour* initState);
	~StateMachine();

	void pushState(CubeBehaviour* state);
	void popState();
	void replaceState(CubeBehaviour* state);

	CubeBehaviour& currentState() const;

	void update(float deltaTime);

private:
	Cube& m_cube;
	std::stack<CubeBehaviour*> m_states;
};

class IdleBehaviour : public CubeBehaviour
{
public:
	IdleBehaviour(Cube &cube);
	virtual ~IdleBehaviour();
	virtual void update(float deltaTime) override;
};

class RollingBehaviour : public CubeBehaviour
{
public:
	RollingBehaviour(Cube &cube, glm::ivec2 dir);
	virtual ~RollingBehaviour();
	virtual void update(float deltaTime) override;

private:
	glm::ivec2 m_dir;
};

#endif