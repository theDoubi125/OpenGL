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
	Shader* m_shader;
	float *m_vertices;
	float *m_textCoords;
	float m_time;
	GLuint m_texture;
	GLuint m_modelMatrixId, m_viewMatrixId, m_projMatrixId;
};

#endif