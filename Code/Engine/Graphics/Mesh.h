#ifndef MESH_H
#define MESH_H

#ifdef EAE6320_PLATFORM_GL
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include <gl/GLU.h>

#endif

#ifdef EAE6320_PLATFORM_D3D
#include <d3d9.h>
#endif

#include <cstdint>

namespace eae6320
{
	class Mesh
	{
	public:
		// Default Constructor for the Mesh class
		Mesh();

		// Parameterized Constructor for each platform
#ifdef EAE6320_PLATFORM_D3D
		// Parameterized Constructor for Direct3D
		Mesh(IDirect3DDevice9* i_direct3dDevice);
#endif
		// Destructor for the Mesh class
		~Mesh();
		void DrawMesh(int vertexCountToRender, int primitiveCountToRender);
		bool LoadMeshFromFile(const char* i_FilePath);
		void LoadVertexAndIndicesData(float ** positions, uint8_t ** colors, uint32_t* i_indexData, int vertexCount, int triangleCount);

	private:
#ifdef EAE6320_PLATFORM_GL
		// A vertex array encapsulates both the vertex and index data as well as the vertex format
		GLuint m_vertexArrayId;

		bool CreateVertexArray(float ** positions, uint8_t ** colors, uint32_t* i_indexData, int vertexCount);
#endif

#ifdef EAE6320_PLATFORM_D3D
		IDirect3DVertexDeclaration9* m_vertexDeclaration;

		// The vertex buffer holds the data for each vertex
		IDirect3DVertexBuffer9* m_vertexBuffer;
		// An index buffer describes how to make triangles with the vertices
		// (i.e. it defines the vertex connectivity)
		IDirect3DIndexBuffer9* m_indexBuffer;

		IDirect3DDevice9* m_direct3dDevice;

		HRESULT GetVertexProcessingUsage(DWORD& o_usage);

		bool CreateVertexBuffer(float ** positions, uint8_t ** colors, int vertexCount);

		bool CreateIndexBuffer(uint32_t* i_indexData, const unsigned int triangleCount);
#endif
	};
}

#endif