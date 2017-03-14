#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <map>

using namespace glm;

enum CellType
{
	CELL_AIR,
	CELL_WALL,
	CELL_LAST,
};

class WorldMesh;
class World : public Entity
{
public:
	World(Scene* scene, ivec3 dim);
	~World();

	void setCell(ivec3 pos, CellType cell);
	const CellType getCell(ivec3 pos) const;

	virtual void init(json descr) override;
	virtual void update(float deltaTime) override;
	virtual void render() const override;

	std::vector<ivec3> getOccurences(CellType cellType) const;

protected:
	int posToId(ivec3 pos) const;

private:
	ivec3 m_dim;
	std::vector<Entity*> m_entities;
	
	std::vector<ivec3>* m_occurences;
	Mesh** m_cellMeshes;
	CellType* m_cells;
	GLuint m_texture;
};

class WorldMesh : public Mesh
{
public:
	WorldMesh(const World &world, CellType type);
	virtual ~WorldMesh();

	virtual void init(json descr) override;
	virtual int vertexCount() const override;
	virtual float* getVertex() const override;
	virtual float* getTexCoords() const override;

private:
	float *m_vertices;
	float *m_textCoords;
	const World &m_world;
	int m_verticeCount;
	CellType m_cellType;

};

#endif WORLD_INCLUDED