#include "DebugSphere.h"

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
			void DebugSphere::CreateSphere(float radius, UINT slices, UINT stacks, uint8_t r, uint8_t g, uint8_t b, D3DVECTOR origin)
			{
				IDirect3DDevice9 *pDirect3DDevice = eae6320::Graphics::Context::getDirect3DDevice();

				// Create the sphere mesh
				ID3DXMesh *pTempSphereMesh;
				ID3DXBuffer *sphereBuffer;
				HRESULT result = D3DXCreateSphere(pDirect3DDevice, radius, slices, stacks, &pTempSphereMesh, &sphereBuffer);
				assert(SUCCEEDED(result));

				// Clone the mesh to allow color
				pTempSphereMesh->CloneMeshFVF(0, D3DFVF_XYZ | D3DFVF_DIFFUSE, pDirect3DDevice, &m_sphereMesh);

				if (SUCCEEDED(m_sphereMesh->GetVertexBuffer(&m_sphereVertexBuffer)))
				{
					int nNumVerts = m_sphereMesh->GetNumVertices();
					sDebugVertex *pVertices = NULL;

					m_sphereVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
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
					m_sphereVertexBuffer->Unlock();

					m_sphereVertexBuffer->Release();
				}
			}

			void DebugSphere::DrawSphere()
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
					result = m_direct3dDevice->SetStreamSource(streamIndex, m_sphereVertexBuffer, bufferOffset, bufferStride);
					assert(SUCCEEDED(result));
				}

				// Bind a specific index buffer to the device as a data source
				{
					IDirect3DIndexBuffer9 *pSphereIndexBuffer;
					result = m_sphereMesh->GetIndexBuffer(&pSphereIndexBuffer);
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
				result = m_direct3dDevice->DrawIndexedPrimitive(primitiveType, 0, 0, m_sphereMesh->GetNumVertices(), 0, m_sphereMesh->GetNumFaces());
				assert(SUCCEEDED(result));
			}
		}
	}
}