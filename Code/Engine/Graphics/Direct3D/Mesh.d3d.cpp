#include "../Mesh.h"

#include <cassert>

// Initialize all member variables
eae6320::Mesh::Mesh() : 
	m_vertexDeclaration(NULL),
	m_vertexBuffer(NULL),
	m_indexBuffer(NULL),
	m_direct3dDevice(NULL)
{

}

eae6320::Mesh::Mesh(IDirect3DVertexDeclaration9* i_vertexDeclaration,
	IDirect3DVertexBuffer9* i_vertexBuffer,
	IDirect3DIndexBuffer9* i_indexBuffer,
	IDirect3DDevice9* i_direct3dDevice) : 
	m_vertexDeclaration(i_vertexDeclaration),
	m_vertexBuffer(i_vertexBuffer),
	m_indexBuffer(i_indexBuffer),
	m_direct3dDevice(i_direct3dDevice)
{

}

eae6320::Mesh::~Mesh()
{
	m_vertexDeclaration = NULL;
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_direct3dDevice = NULL;
}

void eae6320::Mesh::DrawMesh(int vertexCountToRender, int primitiveCountToRender)
{
	// We are using triangles as the "primitive" type,
	// and we have defined the vertex buffer as a triangle list
	// (meaning that every triangle is defined by three vertices)
	const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
	// It's possible to start rendering primitives in the middle of the stream
	const unsigned int indexOfFirstVertexToRender = 0;
	const unsigned int indexOfFirstIndexToUse = 0;
	// We are drawing a square
	HRESULT result = m_direct3dDevice->DrawIndexedPrimitive(primitiveType,
		indexOfFirstVertexToRender, indexOfFirstVertexToRender, (UINT)vertexCountToRender,
		indexOfFirstIndexToUse, (UINT)primitiveCountToRender);
	assert(SUCCEEDED(result));

}
