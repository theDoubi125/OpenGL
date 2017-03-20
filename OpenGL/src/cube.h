#ifndef CUBE_INCLUDED
#define CUBE_INCLUDED

#include <stack>
#include "entity.h"

class CubeBehaviour;
class StateMachine : public Component
{
public:
	StateMachine(Entity* entity);
	~StateMachine();

	virtual void init(json descr) override;
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void render() const override;
	virtual Component* createInstance(Entity* entity) const override;
	virtual std::string name() const override;

	void pushState(CubeBehaviour* state);
	void popState();
	void replaceState(CubeBehaviour* state);

	CubeBehaviour& currentState() const;

	Transform& transform();

private:
	std::stack<CubeBehaviour*> m_states;
};

class CubeBehaviour
{
public:
	CubeBehaviour();
	virtual ~CubeBehaviour();
	void setStateMachine(StateMachine *machine);
	virtual void enter() = 0;
	virtual void update(float deltaTime) = 0;
protected:
	StateMachine *m_machine;
};

class IdleBehaviour : public CubeBehaviour
{
public:
	IdleBehaviour();
	virtual ~IdleBehaviour();
	virtual void update(float deltaTime) override;
	virtual void enter() override;
};

class RollingBehaviour : public CubeBehaviour
{
public:
	RollingBehaviour(glm::ivec2 dir);
	virtual ~RollingBehaviour();
	virtual void update(float deltaTime) override;
	virtual void enter() override;

private:
	glm::ivec2 m_dir;
	glm::vec3 m_initialPos, m_rotCenter;
	float m_stepDuration, m_stepDist, m_time;
	float m_angle;
};

#endif
