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
				void MoveSphere(float i_x, float i_y, float i_z);

				D3DVECTOR GetOrigin() { return m_origin; }
			private:
				D3DVECTOR m_origin;

				IDirect3DVertexDeclaration9* m_vertexDeclaration;
				ID3DXMesh *m_sphereMesh;
				IDirect3DVertexBuffer9 *m_sphereVertexBuffer;
			};
		}
	}
}