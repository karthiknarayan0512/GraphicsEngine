#include "Sprite.h"

#include "../Math/cMatrix_transformation.h"
#include "../UserOutput/UserOutput.h"
#include "Mesh.h"
#include "Context.h"

#include <cassert>

namespace eae6320
{
	namespace Graphics
	{

		bool AlmostEqual(float A, float B, float maxrelativeerror)
		{
			if (A == B)
				return true;
			float relativeError;
			if (fabs(B) > fabs(A))
				relativeError = fabs((A - B) / B);
			else
				relativeError = fabs((A - B) / A);
			if (relativeError <= maxrelativeerror)
				return true;
			return false;
		}

		void Sprite::UpdateUVs(float i_URate, float i_vRate)
		{
			// Fill the vertex buffer with the triangle's vertices
			{
				// Before the vertex buffer can be changed it must be "locked"
				Mesh::sVertex* vertexData = new Mesh::sVertex[6];
				{
					const unsigned int lockEntireBuffer = 0;
					const DWORD useDefaultLockingBehavior = 0;
					const HRESULT result = m_VertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
						reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to lock the vertex buffer");
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
					
					for (int i = 0; i < 6; i++)
					{
						if (vertexData[i].u + i_URate < 1.0f || AlmostEqual(vertexData[i].u + i_URate, 1.0f, 0.00001))
							vertexData[i].u += i_URate;
						else
						{
							vertexData[i].v += i_vRate;
							vertexData[i].u = 0.0f;
						}
					}

					if (AlmostEqual(vertexData[0].u, 1.0f, 0.00001))
					{
						vertexData[0].u = 0.0f;
						vertexData[0].v = m_v;
						vertexData[1].u = m_u;
						vertexData[1].v = 0.0f;
						vertexData[2].u = m_u;
						vertexData[2].v = m_v;

						vertexData[3].u = 0.0f;
						vertexData[3].v = m_v;
						vertexData[4].u = 0.0f;
						vertexData[4].v = 0.0f;
						vertexData[5].u = m_u;
						vertexData[5].v = 0.0f;
					}

				}
				// The buffer must be "unlocked" before it can be used
				{
					const HRESULT result = m_VertexBuffer->Unlock();
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to unlock the vertex buffer");
					}
				}
			}
		}

		void Sprite::UpdateTexture(float left, float right, float top, float bottom)
		{
			{
				// Before the vertex buffer can be changed it must be "locked"
				Mesh::sVertex* vertexData = new Mesh::sVertex[6];
				{
					const unsigned int lockEntireBuffer = 0;
					const DWORD useDefaultLockingBehavior = 0;
					const HRESULT result = m_VertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
						reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to lock the vertex buffer");
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

					vertexData[0].x = left;
					vertexData[0].y = bottom;
					vertexData[1].x = right;
					vertexData[1].y = top;
					vertexData[2].x = right;
					vertexData[2].y = bottom;

					vertexData[3].x = left;
					vertexData[3].y = bottom;
					vertexData[4].x = left;
					vertexData[4].y = top;
					vertexData[5].x = right;
					vertexData[5].y = top;
				}
				// The buffer must be "unlocked" before it can be used
				{
					const HRESULT result = m_VertexBuffer->Unlock();
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to unlock the vertex buffer");
					}
				}
			}
		}

		void Sprite::CreateTexture(const char *i_MaterialPath, float left, float right, float top, float bottom, float u, float v)
		{
			m_u = u;
			m_v = v;
			IDirect3DDevice9* m_direct3dDevice = Context::getDirect3DDevice();
			// The usage tells Direct3D how this vertex buffer will be used
			DWORD usage = 0;
			{
				D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
				const HRESULT result = m_direct3dDevice->GetCreationParameters(&deviceCreationParameters);
				if (SUCCEEDED(result))
				{
					DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
						(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
					usage = (vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? 0 : D3DUSAGE_SOFTWAREPROCESSING;
				}
				else
				{
					eae6320::UserOutput::Print("Direct3D failed to get the device's creation parameters");
				}

				// Our code will only ever write to the buffer
				usage |= D3DUSAGE_WRITEONLY;
			}

			// Create a vertex buffer
			{
				const unsigned int bufferSize = 6 * sizeof(Mesh::sVertex);
				// We will define our own vertex format
				const DWORD useSeparateVertexDeclaration = 0;
				// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
				const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
				HANDLE* const notUsed = NULL;
				const HRESULT result = m_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
					&m_VertexBuffer, notUsed);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create a vertex buffer");
				}
			}

			// Fill the vertex buffer with the triangle's vertices
			{
				// Before the vertex buffer can be changed it must be "locked"
				Mesh::sVertex* vertexData = new Mesh::sVertex[6];
				{
					const unsigned int lockEntireBuffer = 0;
					const DWORD useDefaultLockingBehavior = 0;
					const HRESULT result = m_VertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
						reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to lock the vertex buffer");
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

					for (int i = 0; i < 6; i++)
					{
						vertexData[i].a = 255;
						vertexData[i].r = 255;
						vertexData[i].g = 255;
						vertexData[i].b = 255;
						vertexData[i].z = 0.0f;
					}
					
					vertexData[0].x = left;
					vertexData[0].y = bottom;
					vertexData[0].u = 0.0f;
					vertexData[0].v = v;
					vertexData[1].x = right;
					vertexData[1].y = top;
					vertexData[1].u = u;
					vertexData[1].v = 0.0f;
					vertexData[2].x = right;
					vertexData[2].y = bottom;
					vertexData[2].u = u;
					vertexData[2].v = v;

					vertexData[3].x = left;
					vertexData[3].y = bottom;
					vertexData[3].u = 0.0f;
					vertexData[3].v = v;
					vertexData[4].x = left;
					vertexData[4].y = top;
					vertexData[4].u = 0.0f;
					vertexData[4].v = 0.0f;
					vertexData[5].x = right;
					vertexData[5].y = top;
					vertexData[5].u = u;
					vertexData[5].v = 0.0f;
				}
				// The buffer must be "unlocked" before it can be used
				{
					const HRESULT result = m_VertexBuffer->Unlock();
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to unlock the vertex buffer");
					}
				}
			}
			m_textureMaterial.LoadMaterial(i_MaterialPath);
		}

		void Sprite::SetSprite(Camera &i_Camera)
		{
			Math::cMatrix_transformation i_localToWorldTransform(m_Orientation, m_position);
			m_textureMaterial.SetMaterial(i_localToWorldTransform, i_Camera, false);

			IDirect3DDevice9* m_direct3dDevice = Context::getDirect3DDevice();

			m_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_direct3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_direct3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


			HRESULT result;
			// Bind a specific vertex buffer to the device as a data source
			{
				// There can be multiple streams of data feeding the display adaptor at the same time
				const unsigned int streamIndex = 0;
				// It's possible to start streaming data in the middle of a vertex buffer
				const unsigned int bufferOffset = 0;
				// The "stride" defines how large a single vertex is in the stream of data
				const unsigned int bufferStride = sizeof(Mesh::sVertex);
				result = m_direct3dDevice->SetStreamSource(streamIndex, m_VertexBuffer, bufferOffset, bufferStride);
				assert(SUCCEEDED(result));
			}

			// We are using triangles as the "primitive" type,
			// and we have defined the vertex buffer as a triangle list
			// (meaning that every triangle is defined by three vertices)
			const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
			// It's possible to start rendering primitives in the middle of the stream
			const unsigned int indexOfFirstVertexToRender = 0;

			// We are drawing a square
			result = m_direct3dDevice->DrawPrimitive(primitiveType, indexOfFirstVertexToRender, 2);
			assert(SUCCEEDED(result));
		}
	}
}