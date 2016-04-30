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
	namespace Graphics
	{
		class Mesh
		{
		public:
			struct sVertex
			{
				// POSITION
				// 3 floats == 12 bytes
				// Offset = 0
				float x, y, z;
				// COLOR0
				// 4 uint8_ts == 4 bytes
				// Offset = 12
#ifdef EAE6320_PLATFORM_GL
				uint8_t r, g, b, a;
#elif defined EAE6320_PLATFORM_D3D
				uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#endif
				// TEXCOORD
				// 2 floats == 8 bytes
				// Offset = 16
				float u, v;
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
			void DrawMesh();
			bool LoadMeshFromFile(const char* i_FilePath);
			void LoadVertexAndIndicesData(sVertex* vertices, uint32_t* i_indexData, int vertexCount, int triangleCount);

			IDirect3DVertexBuffer9* GetVertexBuffer() { return m_vertexBuffer; }
			IDirect3DIndexBuffer9* GetIndexBuffer() { return m_indexBuffer; }
			int getVertexCount() { return m_verticesCount; }
			int getIndicesCount() { return m_indicesCount; }

		private:
			int m_verticesCount;
			int m_indicesCount;
#ifdef EAE6320_PLATFORM_GL
			// A vertex array encapsulates both the vertex and index data as well as the vertex format
			GLuint m_vertexArrayId;

			bool CreateVertexArray(sVertex *vertices, uint32_t* i_indexData, int vertexCount, int triangleCount);
#endif

#ifdef EAE6320_PLATFORM_D3D
			IDirect3DVertexDeclaration9* m_vertexDeclaration;

			// The vertex buffer holds the data for each vertex
			IDirect3DVertexBuffer9* m_vertexBuffer;
			// An index buffer describes how to make triangles with the vertices
			// (i.e. it defines the vertex connectivity)
			IDirect3DIndexBuffer9* m_indexBuffer;

			HRESULT GetVertexProcessingUsage(DWORD& o_usage);

			bool CreateVertexBuffer(sVertex* vertices, int vertexCount);

			bool CreateIndexBuffer(uint32_t* i_indexData, const unsigned int triangleCount);
#endif
		};
	}
}

#endif