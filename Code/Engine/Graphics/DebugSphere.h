#pragma once
#include <d3d9.h>
#include <D3DX9Mesh.h>
#include <stdint.h>

namespace eae6320
{
	namespace Graphics
	{
		namespace DebugShapes
		{
			class DebugSphere
			{
			public:
				void DrawSphere();
				void CreateSphere(float radius, UINT slices, UINT stacks, uint8_t r, uint8_t g, uint8_t b, D3DVECTOR origin);
			private:
				IDirect3DVertexDeclaration9* m_vertexDeclaration;
				ID3DXMesh *m_sphereMesh;
				IDirect3DVertexBuffer9 *m_sphereVertexBuffer;
			};
		}
	}
}