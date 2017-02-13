#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "scene.h"
class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void init();

	virtual int vertexCount() const = 0;
	virtual float* getVertex() const = 0;
	virtual float* getTexCoords() const = 0;

	void render() const;


protected:
	float *m_vertices;
	float *m_textCoords;
	GLuint m_vboId;
};

class CubeMesh : public Mesh
{
public:
	CubeMesh();
	virtual ~CubeMesh();

	virtual void init() override;
	virtual int vertexCount() const override;
	virtual float* getVertex() const override;
	virtual float* getTexCoords() const override;
};

#endif MESH_INCLUDED