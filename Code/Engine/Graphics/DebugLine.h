#ifndef DEBUGLINE_H
#define DEBUGLINE_H

#include "DebugShapeVertexStructure.h"

#include <d3d9.h>

namespace eae6320
{
	namespace Graphics
	{
		namespace DebugShapes
		{
			class DebugLine
			{
			public:
				void AddLine();
				void CreateLine(const D3DVECTOR& i_start, const D3DVECTOR& i_end, const uint8_t& b, const uint8_t&g, const uint8_t&r);
			private:
				int m_verticesCount;
				int m_indicesCount;

				IDirect3DVertexDeclaration9* m_vertexDeclaration;

				// The vertex buffer holds the data for each vertex
				IDirect3DVertexBuffer9* m_vertexBuffer;

				HRESULT GetVertexProcessingUsage(DWORD& o_usage);

				bool CreateVertexBuffer(sDebugVertex* vertices);
			};
		}
	}
}

#endif