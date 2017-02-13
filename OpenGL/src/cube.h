#ifndef CUBE_INCLUDED
#define CUBE_INCLUDED

#include "entity.h"
class Cube : public Entity
{
public:
	Cube(const Scene* scene);
	Cube(const Cube &model);
	~Cube();

	virtual void init() override;
	virtual void render() const override;
	virtual void update(const float &deltaTime) override;

private:
	float m_time;
	GLuint m_texture;
};

#endif