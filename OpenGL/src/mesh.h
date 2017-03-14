#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "scene.h"
#include "component.h"

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

	static void registerMesh(std::string name, Mesh* mesh);
	static void registerMeshes();
	static Mesh* getRegisteredMesh(std::string name);

protected:
	GLuint m_vboId;

	static std::map<std::string, Mesh*> m_registeredMeshes;
};

class MeshRenderer : public Component
{
public:
	MeshRenderer(Entity* entity);
	~MeshRenderer();
	virtual void init(json descr) override;
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void render() const override;
	virtual Component* createInstance(Entity* entity) const override;

private:
	Mesh* m_mesh;
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
