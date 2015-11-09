#include "..\Effect.h"
#include "..\..\UserOutput\UserOutput.h"
#include "..\Graphics.h"
#include "..\Context.h"
#include "..\..\Math\Functions.h"

#include <sstream>
#include <fstream>
#include <cassert>

namespace eae6320
{
	namespace Graphics
	{
		Effect::~Effect()
		{
			if (m_vertexShader)
			{
				m_vertexShader->Release();
				m_vertexShader = NULL;
			}
			if (m_fragmentShader)
			{
				m_fragmentShader->Release();
				m_fragmentShader = NULL;
			}
		}

		bool Effect::LoadVertexShader(const char *i_vertexShaderFile)
		{
			// Load the source code from file
			std::ifstream vertexShaderBinary(i_vertexShaderFile, std::ofstream::binary);

			vertexShaderBinary.seekg(0, vertexShaderBinary.end);
			int length = (int)vertexShaderBinary.tellg();
			vertexShaderBinary.seekg(0, vertexShaderBinary.beg);

			char * buffer = new char[length];
			vertexShaderBinary.read(buffer, length);

			//Get the Direct3D device
			IDirect3DDevice9* direct3DDevice = Context::getDirect3DDevice();

			// Create the vertex shader object
			bool wereThereErrors = false;
			{
				HRESULT result = direct3DDevice->CreateVertexShader(reinterpret_cast<DWORD*>(buffer),
					&m_vertexShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
					wereThereErrors = true;
				}
			}

			D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(buffer), &m_vertexShaderConstantTable);

			m_LocalToWorldTransform = m_vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_localToWorld");
			m_WorldToViewTransform = m_vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_worldToView");
			m_ViewToScreenTransform = m_vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_viewToScreen");

			delete buffer;

			return !wereThereErrors;
		}

		bool Effect::LoadFragmentShader(const char *i_fragmentShaderfile)
		{
			// Load the source code from file
			std::ifstream fragmentShaderBinary(i_fragmentShaderfile, std::ofstream::binary);

			fragmentShaderBinary.seekg(0, fragmentShaderBinary.end);
			int length = (int)fragmentShaderBinary.tellg();
			fragmentShaderBinary.seekg(0, fragmentShaderBinary.beg);

			char * buffer = new char[length];
			fragmentShaderBinary.read(buffer, length);

			//Get the Direct3D device
			IDirect3DDevice9* direct3DDevice = Context::getDirect3DDevice();

			// Create the fragment shader object
			bool wereThereErrors = false;
			{
				HRESULT result = direct3DDevice->CreatePixelShader(reinterpret_cast<DWORD*>(buffer),
					&m_fragmentShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
					wereThereErrors = true;
				}
			}
			return !wereThereErrors;
		}

		void Effect::SetEffect()
		{
			//Get the Direct3D device
			IDirect3DDevice9* direct3DDevice = Context::getDirect3DDevice();

			HRESULT result;
			result = direct3DDevice->SetVertexShader(m_vertexShader);
			assert(SUCCEEDED(result));

			result = direct3DDevice->SetPixelShader(m_fragmentShader);
			assert(SUCCEEDED(result));

		}

		void Effect::SetTransforms(Math::cMatrix_transformation i_localToWorldTransform, Camera &i_Camera)
		{
			IDirect3DDevice9* direct3DDevice = Context::getDirect3DDevice();
			HRESULT result = m_vertexShaderConstantTable->SetMatrixTranspose(direct3DDevice, m_LocalToWorldTransform,
				reinterpret_cast<const D3DXMATRIX*>(&i_localToWorldTransform));
			assert(SUCCEEDED(result));

			Math::cMatrix_transformation i_WorldToViewTransform;
			i_WorldToViewTransform = Math::cMatrix_transformation::CreateWorldToViewTransform(i_Camera.getCameraOrientation(), i_Camera.getCameraPosition());
			result = m_vertexShaderConstantTable->SetMatrixTranspose(direct3DDevice, m_WorldToViewTransform,
				reinterpret_cast<const D3DXMATRIX*>(&i_WorldToViewTransform));
			assert(SUCCEEDED(result));

			Math::cMatrix_transformation i_ViewToScreenTransform;
			float i_FOV_y = Math::ConvertDegreesToRadians(60);
			i_ViewToScreenTransform = Math::cMatrix_transformation::CreateViewToScreenTransform(i_FOV_y, Context::getAspectRatio(), 0.1F, 100.0F);
			result = m_vertexShaderConstantTable->SetMatrixTranspose(direct3DDevice, m_ViewToScreenTransform,
				reinterpret_cast<const D3DXMATRIX*>(&i_ViewToScreenTransform));
			assert(SUCCEEDED(result));
		}

		bool Effect::CreateEffect(const char *i_shaderBinaryFile)
		{
			std::ifstream effectBinary(i_shaderBinaryFile, std::ofstream::binary);

			effectBinary.seekg(0, effectBinary.end);
			int length = (int)effectBinary.tellg();
			effectBinary.seekg(0, effectBinary.beg);

			char * buffer = new char[length];
			effectBinary.read(buffer, length);

			char* i_vertexShaderFile = new char[strlen(buffer) + 1];
			memcpy_s(i_vertexShaderFile, strlen(buffer), buffer, strlen(buffer));
			i_vertexShaderFile[strlen(buffer)] = '\0';

			effectBinary.seekg(strlen(i_vertexShaderFile) + 1, effectBinary.beg);
			effectBinary.read(buffer, length - strlen(i_vertexShaderFile) - 1);

			char* i_fragmentShaderfile = new char[strlen(buffer) + 1];
			memcpy_s(i_fragmentShaderfile, strlen(buffer), buffer, strlen(buffer));
			i_fragmentShaderfile[strlen(buffer)] = '\0';

			effectBinary.close();

			if (!LoadVertexShader(i_vertexShaderFile))
				return false;

			if (!LoadFragmentShader(i_fragmentShaderfile))
				return false;

			return true;
		}
	}
}