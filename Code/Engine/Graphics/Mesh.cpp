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
	int verticesCount = 0;

	// Get indices data
	uint32_t* indexData = NULL;
	int indicesCount = 0;

	// Get vertexCount
	verticesCount = *reinterpret_cast<int *>(buffer);

	// Get vertices
	vertices = reinterpret_cast<sVertex *>(buffer + sizeof(verticesCount));

	//Write indices count
	indicesCount = *reinterpret_cast<int *>(buffer + sizeof(verticesCount) + 
											verticesCount * sizeof(sVertex));

	// Write index array
	indexData = reinterpret_cast<uint32_t*>(buffer + sizeof(verticesCount) +
											verticesCount * sizeof(sVertex) +
											sizeof(indicesCount));

	LoadVertexAndIndicesData(vertices, indexData, verticesCount, indicesCount);

	delete buffer;
	meshBinary.close();

	vertices = NULL;
	indexData = NULL;

	return !wereThereErrors;
}