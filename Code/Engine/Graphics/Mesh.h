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
		struct sVertex
		{
			// POSITION
			// 2 floats == 8 bytes
			// Offset = 0
			float x, y;
			// COLOR0
			// 4 uint8_ts == 4 bytes
			// Offset = 8
#ifdef EAE6320_PLATFORM_GL
			uint8_t r, g, b, a;
#elif defined EAE6320_PLATFORM_D3D
			uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#endif
		};

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
		void LoadVertexAndIndicesData(sVertex* vertices, uint32_t* i_indexData, int vertexCount, int triangleCount);

	private:
#ifdef EAE6320_PLATFORM_GL
		// A vertex array encapsulates both the vertex and index data as well as the vertex format
		GLuint m_vertexArrayId;

		bool CreateVertexArray(sVertex *vertices, uint32_t* i_indexData, int vertexCount);
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

		bool CreateVertexBuffer(sVertex* vertices, int vertexCount);

		bool CreateIndexBuffer(uint32_t* i_indexData, const unsigned int triangleCount);
#endif
	};
}

#endif