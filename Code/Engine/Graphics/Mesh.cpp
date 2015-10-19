#include <iostream>
#include <fstream>
#include <cassert>

#include "Mesh.h"

bool eae6320::Mesh::LoadMeshFromFile(const char* i_FilePath)
{
	bool wereThereErrors = false;

	std::ifstream meshBinary(i_FilePath, std::ofstream::binary);
	
	meshBinary.seekg(0, meshBinary.end);
	int length = (int)meshBinary.tellg();
	meshBinary.seekg(0, meshBinary.beg);

	char * buffer = new char[length];
	meshBinary.read(buffer, length);

	sVertex *vertices = NULL;

	// Get indices data
	uint32_t* indexData = NULL;

	// Get vertexCount
	m_verticesCount = *reinterpret_cast<int *>(buffer);

	// Get vertices
	vertices = reinterpret_cast<sVertex *>(buffer + sizeof(m_verticesCount));

	//Write indices count
	m_indicesCount = *reinterpret_cast<int *>(buffer + sizeof(m_verticesCount) +
		m_verticesCount * sizeof(sVertex));

	// Write index array
	indexData = reinterpret_cast<uint32_t*>(buffer + sizeof(m_verticesCount) +
		m_verticesCount * sizeof(sVertex) +
											sizeof(m_indicesCount));

	LoadVertexAndIndicesData(vertices, indexData, m_verticesCount, m_indicesCount);

	delete buffer;
	meshBinary.close();

	vertices = NULL;
	indexData = NULL;

	return !wereThereErrors;
}