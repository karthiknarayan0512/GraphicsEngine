#pragma once
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
			class DebugBox
			{
			public:
				void DrawBox();
				void CreateBox(float width, float height, float depth, uint8_t r, uint8_t g, uint8_t b, D3DVECTOR origin);
			private:
				IDirect3DVertexDeclaration9* m_vertexDeclaration;
				ID3DXMesh *m_boxMesh;
				IDirect3DVertexBuffer9 *m_boxVertexBuffer;
			};
		}
	}
}