#ifndef MESH_H
#define MESH_H

#ifdef EAE6320_PLATFORM_GL
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include <gl/GLU.h>

#endif

#ifdef EAE6320_PLATFORM_D3D
#include <d3d9.h>
#endif

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
		Mesh(IDirect3DVertexDeclaration9* i_vertexDeclaration, 
			 IDirect3DVertexBuffer9* i_vertexBuffer,
			IDirect3DIndexBuffer9* i_indexBuffer,
			IDirect3DDevice9* i_direct3dDevice);
#endif

#ifdef EAE6320_PLATFORM_GL
		// Parameterized Constructor for OpenGL
		Mesh(GLuint i_vertexArrayId);
#endif


		// Destructor for the Mesh class
		~Mesh();
		void DrawMesh(int vertexCountToRender, int primitiveCountToRender);

	private:
#ifdef EAE6320_PLATFORM_GL
		// A vertex array encapsulates both the vertex and index data as well as the vertex format
		GLuint m_vertexArrayId;
#endif

#ifdef EAE6320_PLATFORM_D3D
		IDirect3DVertexDeclaration9* m_vertexDeclaration;

		// The vertex buffer holds the data for each vertex
		IDirect3DVertexBuffer9* m_vertexBuffer;
		// An index buffer describes how to make triangles with the vertices
		// (i.e. it defines the vertex connectivity)
		IDirect3DIndexBuffer9* m_indexBuffer;

		IDirect3DDevice9* m_direct3dDevice;
#endif
	};
}

#endif