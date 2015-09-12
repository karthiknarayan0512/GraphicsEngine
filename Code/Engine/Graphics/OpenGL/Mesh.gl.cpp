#include "..\Mesh.h"

#include <cassert>

// Initialize all member variables
eae6320::Mesh::Mesh() :
	m_vertexArrayId(0)
{

}

eae6320::Mesh::Mesh(GLuint i_vertexArrayId)
{
	m_vertexArrayId = i_vertexArrayId;
}

eae6320::Mesh::~Mesh()
{

}

void eae6320::Mesh::DrawMesh(int vertexCountToRender, int primitiveCountToRender)
{
	// We are using triangles as the "primitive" type,
	// and we have defined the vertex buffer as a triangle list
	// (meaning that every triangle is defined by three vertices)
	const GLenum mode = GL_TRIANGLES;
	// We'll use 32-bit indices in this class to keep things simple
	// (i.e. every index will be a 32 bit unsigned integer)
	const GLenum indexType = GL_UNSIGNED_INT;
	// It is possible to start rendering in the middle of an index buffer
	const GLvoid* const offset = 0;
	// We are drawing a square
	glDrawElements(mode, vertexCountToRender, indexType, offset);
	assert(glGetError() == GL_NO_ERROR);

}