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

		Effect::tUniformHandle Effect::getUniformHandle(ShaderTypes::eShaderType i_ShaderType, const char * i_uniformName)
		{
			if (i_ShaderType == ShaderTypes::Fragment)
				return m_fragmentShaderConstantTable->GetConstantByName(NULL, i_uniformName);
			else if (i_ShaderType == ShaderTypes::Vertex)
				return m_vertexShaderConstantTable->GetConstantByName(NULL, i_uniformName);
			else
				return NULL;
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

			D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(buffer), &m_fragmentShaderConstantTable);

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

			uint8_t alpha = 1 << ALPHA_TRANSPARENCY;
			uint8_t	depthTest = 1 << DEPTH_TESTING;
			uint8_t depthwrite = 1 << DEPTH_WRITING;

			// Set alpha transparency
			if (m_renderStates & alpha)
			{
				result = direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				assert(SUCCEEDED(result));

				result = direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				assert(SUCCEEDED(result));

				result = direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				assert(SUCCEEDED(result));
			}
			else
			{
				result = direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				assert(SUCCEEDED(result));
			}

			// Set depth testing
			if (m_renderStates & depthTest)
			{
				result = direct3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				assert(SUCCEEDED(result));

				result = direct3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				assert(SUCCEEDED(result));
			}
			else
			{
				result = direct3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
				assert(SUCCEEDED(result));
			}

			// Set depth writing
			if (m_renderStates & depthwrite)
			{
				result = direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				assert(SUCCEEDED(result));
			}
			else
			{
				result = direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				assert(SUCCEEDED(result));
			}
		}

		void Effect::SetUniforms(ShaderTypes::eShaderType i_ShaderType, tUniformHandle i_uniformHandle, float* i_values, uint8_t i_valueCountToSet)
		{
			IDirect3DDevice9* direct3DDevice = Context::getDirect3DDevice();
			if (i_ShaderType == ShaderTypes::Fragment)
			{
				m_fragmentShaderConstantTable->SetFloatArray(direct3DDevice, i_uniformHandle, i_values, i_valueCountToSet);
			}
			else if (i_ShaderType == ShaderTypes::Vertex)
			{
				m_vertexShaderConstantTable->SetFloatArray(direct3DDevice, i_uniformHandle, i_values, i_valueCountToSet);
			}
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

			buffer += strlen(buffer) + 1;

			char* i_fragmentShaderfile = new char[strlen(buffer) + 1];
			memcpy_s(i_fragmentShaderfile, strlen(buffer), buffer, strlen(buffer));
			i_fragmentShaderfile[strlen(buffer)] = '\0';

			buffer += strlen(buffer) + 1;

			m_renderStates = static_cast<uint8_t>(*buffer);

			effectBinary.close();

			if (!LoadVertexShader(i_vertexShaderFile))
				return false;

			if (!LoadFragmentShader(i_fragmentShaderfile))
				return false;

			return true;
		}
	}
}