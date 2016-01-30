#include "DebugBox.h"

#include "DebugShapeVertexStructure.h"
#include "Context.h"

#include <D3dx9shape.h>
#include <cassert>

namespace eae6320
{
	namespace Graphics
	{
		namespace DebugShapes
		{
			void DebugBox::CreateBox(float width, float height, float depth, uint8_t r, uint8_t g, uint8_t b, D3DVECTOR origin)
			{
				IDirect3DDevice9 *pDirect3DDevice = eae6320::Graphics::Context::getDirect3DDevice();

				// Create the sphere mesh
				ID3DXMesh *pTempBoxMesh;
				ID3DXBuffer *pBoxBuffer;
				HRESULT result = D3DXCreateBox(pDirect3DDevice, width, height, depth, &pTempBoxMesh, &pBoxBuffer);
				assert(SUCCEEDED(result));

				// Clone the mesh to allow color
				pTempBoxMesh->CloneMeshFVF(0, D3DFVF_XYZ | D3DFVF_DIFFUSE, pDirect3DDevice, &m_boxMesh);

				if (SUCCEEDED(m_boxMesh->GetVertexBuffer(&m_boxVertexBuffer)))
				{
					int nNumVerts = m_boxMesh->GetNumVertices();
					sDebugVertex *pVertices = NULL;

					m_boxVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
					{
						for (int i = 0; i < nNumVerts; ++i)
						{
							pVertices[i].r = r;
							pVertices[i].g = g;
							pVertices[i].b = b;
							pVertices[i].a = 255;

							pVertices[i].x += origin.x;
							pVertices[i].y += origin.y;
							pVertices[i].z += origin.z;
						}
					}
					m_boxVertexBuffer->Unlock();
				}
			}

			void DebugBox::DrawBox()
			{
				IDirect3DDevice9* m_direct3dDevice = Context::getDirect3DDevice();
				HRESULT result;
				// Bind a specific vertex buffer to the device as a data source
				{
					// There can be multiple streams of data feeding the display adaptor at the same time
					const unsigned int streamIndex = 0;
					// It's possible to start streaming data in the middle of a vertex buffer
					const unsigned int bufferOffset = 0;
					// The "stride" defines how large a single vertex is in the stream of data
					const unsigned int bufferStride = sizeof(sDebugVertex);
					result = m_direct3dDevice->SetStreamSource(streamIndex, m_boxVertexBuffer, bufferOffset, bufferStride);
					assert(SUCCEEDED(result));
				}

				// Bind a specific index buffer to the device as a data source
				{
					IDirect3DIndexBuffer9 *pBoxIndexBuffer;
					result = m_boxMesh->GetIndexBuffer(&pBoxIndexBuffer);
					assert(SUCCEEDED(result));

					result = m_direct3dDevice->SetIndices(pBoxIndexBuffer);
					assert(SUCCEEDED(result));
				}

				// We are using triangles as the "primitive" type,
				// and we have defined the vertex buffer as a triangle list
				// (meaning that every triangle is defined by three vertices)
				const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
				// It's possible to start rendering primitives in the middle of the stream
				const unsigned int indexOfFirstVertexToRender = 0;
				// We are drawing a square
				result = m_direct3dDevice->DrawIndexedPrimitive(primitiveType, 0, 0, m_boxMesh->GetNumVertices(), 0, m_boxMesh->GetNumFaces());
				assert(SUCCEEDED(result));
			}
		}
	}
}