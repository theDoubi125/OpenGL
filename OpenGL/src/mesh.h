#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "scene.h"
#include "entity.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void init(json descr);

	virtual int vertexCount() const = 0;
	virtual float* getVertex() const = 0;
	virtual float* getTexCoords() const = 0;

	void render() const;


protected:
	GLuint m_vboId;
};

class MeshRenderer : public Component
{
public:
	virtual void init(json descr) override;
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void render() const override;
	virtual Component* clone() const override;
};

class CubeMesh : public Mesh
{
public:
	CubeMesh();
	virtual ~CubeMesh();

	virtual void init(json descr) override;
	virtual int vertexCount() const override;
	virtual float* getVertex() const override;
	virtual float* getTexCoords() const override;
private:
	float *m_vertices;
	float *m_textCoords;
};

#endif MESH_INCLUDED
