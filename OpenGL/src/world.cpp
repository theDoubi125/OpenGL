#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>

#include "entity.h"
#include "shader.h"
#include "scene.h"
#include "world.h"

World::World(ivec3 dim) : m_dim(dim), m_cells(new CellType[dim.x * dim.y * dim.z])
{
	for (int i = 0; i < dim.x * dim.y * dim.z; i++)
		m_cells[i] = CELL_AIR;
	m_occurences = new std::vector<ivec3>[CELL_LAST];
	m_cellMeshes = new Mesh*[CELL_LAST];
	for (int i = CELL_AIR+1; i < CELL_LAST; i++)
	{
		m_cellMeshes[i] = new WorldMesh(*this, (CellType)i);
	}
}

World::~World()
{
	delete[] m_cells;
	delete[] m_occurences;
	delete[] m_cellMeshes;
	for(Entity* entity : m_entities)
	{
		delete entity;
	}
}

void World::setCell(ivec3 pos, CellType cell)
{
	int cellId = posToId(pos);
	if (cellId >= 0)
	{
		m_cells[posToId(pos)] = cell;
		m_occurences[cell].push_back(pos);
	}
}

const CellType World::getCell(ivec3 pos) const
{
	int cellId = posToId(pos);
	if (cellId < 0)
		return CELL_AIR;
	return m_cells[cellId];
}

int World::posToId(ivec3 pos) const
{
	if (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < m_dim.x && pos.y < m_dim.y && pos.z < m_dim.z)
	{
		return pos.x + pos.y * m_dim.x + pos.z * m_dim.x * m_dim.y;
	}
	else return -1;
}

void World::init()
{

}

void World::update(float deltaTime)
{

}

void World::render() const
{
	for (int i = 1; i < CELL_LAST; i++)
	{
		m_cellMeshes[i]->render();
	}
}

std::vector<ivec3> World::getOccurences(CellType cellType) const
{
	return m_occurences[cellType];
}

WorldMesh::WorldMesh(const World &world, CellType cellType) : m_world(world), m_vertices(NULL), m_textCoords(NULL), m_verticeCount(0), m_cellType(cellType)
{

}

WorldMesh::~WorldMesh()
{
	delete[] m_vertices;
	delete[] m_textCoords;
}

void WorldMesh::init()
{
	std::vector<ivec3> pos = m_world.getOccurences(m_cellType);

	float vertexModel[72]
	{
		-0.5, -0.5, -0.5,  0.5, -0.5, -0.5,  0.5, 0.5, -0.5,  -0.5, 0.5, -0.5,
		-0.5, -0.5, 0.5,  0.5, -0.5, 0.5,  0.5, 0.5, 0.5,  -0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,  0.5, -0.5, -0.5,  0.5, -0.5, 0.5,  -0.5, -0.5, 0.5,
		-0.5, 0.5, -0.5,  0.5, 0.5, -0.5,  0.5, 0.5, 0.5,  -0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,  -0.5, -0.5, 0.5,  -0.5, 0.5, 0.5,  -0.5, 0.5, -0.5,
		0.5, -0.5, -0.5,  0.5, -0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, -0.5,
	};
	float textCoordModel[48]
	{
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
		0, 0,   1, 0,  1, 1,  0, 1,
	};

	m_verticeCount = pos.size() * 24;
	m_vertices = new float[72 * pos.size()];
	m_textCoords = new float[48 * pos.size()];

	for (int i = 0; i < pos.size(); i++)
	{
		for (int j = 0; j < 72; j++)
			m_vertices[i * 72 + j] = vertexModel[j];
		for (int j = 0; j < 48; j++)
			m_textCoords[i * 48 + j];
	}
}

int WorldMesh::vertexCount() const
{
	return m_verticeCount;
}

float* WorldMesh::getVertex() const
{
	return m_vertices;
}

float* WorldMesh::getTexCoords() const
{
	return m_textCoords;
}