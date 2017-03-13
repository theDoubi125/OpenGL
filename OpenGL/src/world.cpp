#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SOIL.h>

#include "entity.h"
#include "shader.h"
#include "scene.h"
#include "world.h"
#include "mesh.h"

World::World(Scene* scene, ivec3 dim) : Entity(scene), m_dim(dim), m_cells(new CellType[dim.x * dim.y * dim.z])
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

void World::init(Json::Value descr)
{
	Entity::init(Json::Value());
	for (int i = 1; i < CELL_LAST; i++)
	{
		m_cellMeshes[i]->init(descr);
	}
	m_texture = SOIL_load_OGL_texture
	(
		"resources/img/box.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (0 == m_texture)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
}

void World::update(float deltaTime)
{
	Entity::update(deltaTime);
}

void World::render() const
{
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//Entity::render();
	for (int i = 1; i < CELL_LAST; i++)
	{
		m_cellMeshes[i]->render();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
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

void WorldMesh::init(Json::Value descr)
{
	std::vector<ivec3> pos = m_world.getOccurences(m_cellType);

	float vertexModel[72]
	{
		-0.5, -0.5, -0.5,	-0.5, 0.5, -0.5,	0.5, 0.5, -0.5,		0.5, -0.5, -0.5,
		-0.5, -0.5, 0.5,	0.5, -0.5, 0.5,		0.5, 0.5, 0.5,		-0.5, 0.5, 0.5,
		-0.5, -0.5, -0.5,	0.5, -0.5, -0.5,	0.5, -0.5, 0.5,		-0.5, -0.5, 0.5,
		-0.5, 0.5, -0.5,	-0.5, 0.5, 0.5,		0.5, 0.5, 0.5,		0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,	-0.5, -0.5, 0.5,	-0.5, 0.5, 0.5,		-0.5, 0.5, -0.5,
		0.5, -0.5, -0.5,	0.5, 0.5, -0.5,		0.5, 0.5, 0.5,		0.5, -0.5, 0.5
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

	ivec3 dirVecs[] = { ivec3(0, 0, -1), ivec3(0, 0, 1), ivec3(0, -1, 0), ivec3(0, 1, 0), ivec3(-1, 0, 0), ivec3(1, 0, 0) };

	int faceCount = 0;
	int jmin = 0;
	int jmax = 6;
	for (int i = 0; i < pos.size(); i++)
	{
		for (int j = jmin; j < jmax; j++)
		{
			if (m_world.getCell(pos[i] + dirVecs[j]) != m_cellType)
			{
				faceCount ++;
			}
		}
	}
	m_verticeCount = faceCount * 4;
	m_vertices = new float[m_verticeCount * 3];
	m_textCoords = new float[m_verticeCount * 2];

	int currentFace = 0;
	for (int i = 0; i < pos.size(); i++)
	{
		for (int j = jmin; j < jmax; j++)
		{
			if (m_world.getCell(pos[i] + dirVecs[j]) != m_cellType)
			{
				for (int k = 0; k < 4; k++)
				{
					m_vertices[currentFace * 12 + k * 3] = vertexModel[12 * j + k * 3] + pos[i].x;
					m_vertices[currentFace * 12 + k * 3 + 1] = vertexModel[12 * j + k * 3 + 1] + pos[i].y;
					m_vertices[currentFace * 12 + k * 3 + 2] = vertexModel[12 * j + k * 3 + 2] + pos[i].z;

					m_textCoords[currentFace * 8 + k * 2] = textCoordModel[8 * j + k * 2];
					m_textCoords[currentFace * 8 + k * 2 + 1] = textCoordModel[8 * j + k * 2 + 1];
				}
				currentFace++;
			}
		}
	}
	Mesh::init(descr);
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