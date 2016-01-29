#include "DebugCylinder.h"

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
			void DebugCylinder::CreateCylinder(float radius1, float radius2, float length, UINT slices, UINT stacks, uint8_t r, uint8_t g, uint8_t b, D3DVECTOR origin)
			{
				IDirect3DDevice9 *pDirect3DDevice = eae6320::Graphics::Context::getDirect3DDevice();

				// Create the sphere mesh
				ID3DXMesh *pTempCylinderMesh;
				ID3DXBuffer *pCylinderBuffer;
				HRESULT result = D3DXCreateCylinder(pDirect3DDevice, radius1, radius2, length, slices, stacks, &pTempCylinderMesh, &pCylinderBuffer);
				assert(SUCCEEDED(result));

				// Clone the mesh to allow color
				pTempCylinderMesh->CloneMeshFVF(0, D3DFVF_XYZ | D3DFVF_DIFFUSE, pDirect3DDevice, &m_cylinderMesh);

				if (SUCCEEDED(m_cylinderMesh->GetVertexBuffer(&m_cylinderVertexBuffer)))
				{
					int nNumVerts = m_cylinderMesh->GetNumVertices();
					sDebugVertex *pVertices = NULL;

					m_cylinderVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
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
					m_cylinderVertexBuffer->Unlock();

					m_cylinderVertexBuffer->Release();
				}
			}

			void DebugCylinder::DrawCylinder()
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
					result = m_direct3dDevice->SetStreamSource(streamIndex, m_cylinderVertexBuffer, bufferOffset, bufferStride);
					assert(SUCCEEDED(result));
				}

				// Bind a specific index buffer to the device as a data source
				{
					IDirect3DIndexBuffer9 *pSphereIndexBuffer;
					result = m_cylinderMesh->GetIndexBuffer(&pSphereIndexBuffer);
					assert(SUCCEEDED(result));

					result = m_direct3dDevice->SetIndices(pSphereIndexBuffer);
					assert(SUCCEEDED(result));
				}

				// We are using triangles as the "primitive" type,
				// and we have defined the vertex buffer as a triangle list
				// (meaning that every triangle is defined by three vertices)
				const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
				// It's possible to start rendering primitives in the middle of the stream
				const unsigned int indexOfFirstVertexToRender = 0;
				// We are drawing a square
				result = m_direct3dDevice->DrawIndexedPrimitive(primitiveType, 0, 0, m_cylinderMesh->GetNumVertices(), 0, m_cylinderMesh->GetNumFaces());
				assert(SUCCEEDED(result));
			}
		}
	}
}