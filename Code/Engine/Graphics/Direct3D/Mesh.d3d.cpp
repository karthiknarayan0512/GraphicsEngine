#include "../Mesh.h"
#include "../../UserOutput/UserOutput.h"
#include "..\Graphics.h"
#include "..\Context.h"

#include <cassert>

// Initialize all member variables
namespace eae6320
{
	namespace Graphics
	{
		Mesh::Mesh() :
			m_vertexDeclaration(NULL),
			m_vertexBuffer(NULL),
			m_indexBuffer(NULL)
		{
		}

		Mesh::Mesh(IDirect3DDevice9* i_direct3dDevice) :
			m_vertexDeclaration(NULL),
			m_vertexBuffer(NULL),
			m_indexBuffer(NULL)
		{
		}

		Mesh::~Mesh()
		{
			IDirect3DDevice9* m_direct3dDevice = Context::getDirect3DDevice();
			if (m_direct3dDevice)
			{
				if (m_vertexBuffer)
				{
					m_vertexBuffer->Release();
					m_vertexBuffer = NULL;
				}
				if (m_indexBuffer)
				{
					m_indexBuffer->Release();
					m_indexBuffer = NULL;
				}
				if (m_vertexDeclaration)
				{
					m_direct3dDevice->SetVertexDeclaration(NULL);
					m_vertexDeclaration->Release();
					m_vertexDeclaration = NULL;
				}

				m_direct3dDevice->Release();
			}
		}

		void Mesh::DrawMesh()
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
				const unsigned int bufferStride = sizeof(sVertex);
				result = m_direct3dDevice->SetStreamSource(streamIndex, m_vertexBuffer, bufferOffset, bufferStride);
				assert(SUCCEEDED(result));
			}
			// Bind a specific index buffer to the device as a data source
			{
				result = m_direct3dDevice->SetIndices(m_indexBuffer);
				assert(SUCCEEDED(result));
			}

			// We are using triangles as the "primitive" type,
			// and we have defined the vertex buffer as a triangle list
			// (meaning that every triangle is defined by three vertices)
			const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
			// It's possible to start rendering primitives in the middle of the stream
			const unsigned int indexOfFirstVertexToRender = 0;
			const unsigned int indexOfFirstIndexToUse = 0;
			// We are drawing a square
			result = m_direct3dDevice->DrawIndexedPrimitive(primitiveType,
				indexOfFirstVertexToRender, indexOfFirstVertexToRender, (UINT)m_verticesCount,
				indexOfFirstIndexToUse, (UINT)m_indicesCount);
			assert(SUCCEEDED(result));
		}

		void Mesh::LoadVertexAndIndicesData(sVertex* vertices, uint32_t* i_indexData, int vertexCount, int triangleCount)
		{
			CreateVertexBuffer(vertices, vertexCount);
			CreateIndexBuffer(i_indexData, triangleCount);
		}

		HRESULT Mesh::GetVertexProcessingUsage(DWORD& o_usage)
		{
			IDirect3DDevice9* m_direct3dDevice = Context::getDirect3DDevice();
			D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
			const HRESULT result = m_direct3dDevice->GetCreationParameters(&deviceCreationParameters);
			if (SUCCEEDED(result))
			{
				DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
					(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
				o_usage = (vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? 0 : D3DUSAGE_SOFTWAREPROCESSING;
			}
			else
			{
				eae6320::UserOutput::Print("Direct3D failed to get the device's creation parameters");
			}
			return result;
		}

		bool Mesh::CreateVertexBuffer(sVertex* vertices, int vertexCount)
		{
			IDirect3DDevice9* m_direct3dDevice = Context::getDirect3DDevice();
			// The usage tells Direct3D how this vertex buffer will be used
			DWORD usage = 0;
			{
				// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
				const HRESULT result = GetVertexProcessingUsage(usage);
				if (FAILED(result))
				{
					return false;
				}
				// Our code will only ever write to the buffer
				usage |= D3DUSAGE_WRITEONLY;
			}

			// Initialize the vertex format
			{
				// These elements must match the VertexFormat::sVertex layout struct exactly.
				// They instruct Direct3D how to match the binary data in the vertex buffer
				// to the input elements in a vertex shader
				// (by using D3DDECLUSAGE enums here and semantics in the shader,
				// so that, for example, D3DDECLUSAGE_POSITION here matches with POSITION in shader code).
				// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
				D3DVERTEXELEMENT9 vertexElements[] =
				{
					// Stream 0

					// POSITION
					// 3 floats == 12 bytes
					// Offset = 0
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

					// COLOR0
					// D3DCOLOR == 4 bytes
					// Offset = 12
					{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

					// The following marker signals the end of the vertex declaration
					D3DDECL_END()
				};
				HRESULT result = m_direct3dDevice->CreateVertexDeclaration(vertexElements, &m_vertexDeclaration);
				if (SUCCEEDED(result))
				{
					result = m_direct3dDevice->SetVertexDeclaration(m_vertexDeclaration);
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to set the vertex declaration");
						return false;
					}
				}
				else
				{
					eae6320::UserOutput::Print("Direct3D failed to create a Direct3D9 vertex declaration");
					return false;
				}
			}

			// Create a vertex buffer
			{
				const unsigned int bufferSize = vertexCount * sizeof(sVertex);
				// We will define our own vertex format
				const DWORD useSeparateVertexDeclaration = 0;
				// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
				const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
				HANDLE* const notUsed = NULL;
				const HRESULT result = m_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
					&m_vertexBuffer, notUsed);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create a vertex buffer");
					return false;
				}
			}
			// Fill the vertex buffer with the triangle's vertices
			{
				// Before the vertex buffer can be changed it must be "locked"
				sVertex* vertexData = new sVertex[vertexCount];
				{
					const unsigned int lockEntireBuffer = 0;
					const DWORD useDefaultLockingBehavior = 0;
					const HRESULT result = m_vertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
						reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to lock the vertex buffer");
						return false;
					}
				}
				// Fill the buffer
				{
					// You will need to fill in two pieces of information for each vertex:
					//	* 2 floats for the POSITION
					//	* 4 uint8_ts for the COLOR

					// The floats for POSITION are for the X and Y coordinates, like in Assignment 02.
					// The difference this time is that there should be fewer (because we are sharing data).

					// The uint8_ts for COLOR are "RGBA", where "RGB" stands for "Red Green Blue" and "A" for "Alpha".
					// Conceptually each of these values is a [0,1] value, but we store them as an 8-bit value to save space
					// (color doesn't need as much precision as position),
					// which means that the data we send to the GPU will be [0,255].
					// For now the alpha value should _always_ be 255, and so you will choose color by changing the first three RGB values.
					// To make white you should use (255, 255, 255), to make black (0, 0, 0).
					// To make pure red you would use the max for R and nothing for G and B, so (1, 0, 0).
					// Experiment with other values to see what happens!
					memcpy(vertexData, vertices, sizeof(sVertex) * vertexCount);
				}
				// The buffer must be "unlocked" before it can be used
				{
					const HRESULT result = m_vertexBuffer->Unlock();
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to unlock the vertex buffer");
						return false;
					}
				}
			}

			return true;
		}

		bool Mesh::CreateIndexBuffer(uint32_t* i_indexData, const unsigned int triangleCount)
		{
			IDirect3DDevice9* m_direct3dDevice = Context::getDirect3DDevice();
			// The usage tells Direct3D how this vertex buffer will be used
			DWORD usage = 0;
			{
				// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
				const HRESULT result = GetVertexProcessingUsage(usage);
				if (FAILED(result))
				{
					return false;
				}
				// Our code will only ever write to the buffer
				usage |= D3DUSAGE_WRITEONLY;
			}

			// Create an index buffer
			unsigned int bufferSize;
			{
				// We are drawing a square
				const unsigned int vertexCountPerTriangle = 3;
				bufferSize = triangleCount * vertexCountPerTriangle * sizeof(uint32_t);
				// We'll use 32-bit indices in this class to keep things simple
				// (i.e. every index will be a 32 bit unsigned integer)
				const D3DFORMAT format = D3DFMT_INDEX32;
				// Place the index buffer into memory that Direct3D thinks is the most appropriate
				const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
				HANDLE* notUsed = NULL;
				const HRESULT result = m_direct3dDevice->CreateIndexBuffer(bufferSize, usage, format, useDefaultPool,
					&m_indexBuffer, notUsed);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create an index buffer");
					return false;
				}
			}
			// Fill the index buffer with the triangles' connectivity data
			{
				// Before the index buffer can be changed it must be "locked"
				uint32_t* indexData = new uint32_t[triangleCount];
				{
					const unsigned int lockEntireBuffer = 0;
					const DWORD useDefaultLockingBehavior = 0;
					const HRESULT result = m_indexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
						reinterpret_cast<void**>(&indexData), useDefaultLockingBehavior);
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to lock the index buffer");
						return false;
					}
				}
				// Fill the buffer
				{
					// EAE6320_TODO:
					// You will need to fill in 3 indices for each triangle that needs to be drawn.
					// Each index will be a 32-bit unsigned integer,
					// and will index into the vertex buffer array that you have created.
					// The order of indices is important, but the correct order will depend on
					// which vertex you have assigned to which spot in your vertex buffer
					// (also remember to maintain the correct handedness for the triangle winding order).
					memcpy(indexData, i_indexData, triangleCount * 3 * sizeof(uint32_t));

				}
				// The buffer must be "unlocked" before it can be used
				{
					const HRESULT result = m_indexBuffer->Unlock();
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to unlock the index buffer");
						return false;
					}
				}
			}

			return true;
		}
	}
}