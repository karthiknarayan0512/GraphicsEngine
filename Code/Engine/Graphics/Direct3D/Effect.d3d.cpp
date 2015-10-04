#include "..\Effect.h"
#include "..\..\UserOutput\UserOutput.h"
#include "..\Graphics.h"

#include <d3dx9shader.h>
#include <sstream>
#include <cassert>

namespace eae6320
{
	Effect::Effect(IDirect3DDevice9* i_direct3dDevice) :
		m_direct3dDevice(i_direct3dDevice)
	{

	}

	Effect::~Effect()
	{
		if (m_direct3dDevice)
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

			m_direct3dDevice->Release();
		}
	}

	bool Effect::LoadVertexShader(const char *i_vertexShaderFile)
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const char* sourceCodeFileName = i_vertexShaderFile;
			const D3DXMACRO defines[] =
			{
				{ "EAE6320_PLATFORM_D3D", "1" },
				{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "vs_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants);
			if (SUCCEEDED(result))
			{
				if (errorMessages)
				{
					errorMessages->Release();
				}
			}
			else
			{
				if (errorMessages)
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName
						<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
					eae6320::UserOutput::Print(errorMessage.str());
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName;
					eae6320::UserOutput::Print(errorMessage.str());
				}
				return false;
			}
		}
		// Create the vertex shader object
		bool wereThereErrors = false;
		{
			HRESULT result = m_direct3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
				&m_vertexShader);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}

	bool Effect::LoadFragmentShader(const char *i_fragmentShaderfile)
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const char* sourceCodeFileName = i_fragmentShaderfile;
			const D3DXMACRO defines[] =
			{
				{ "EAE6320_PLATFORM_D3D", "1" },
				{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "ps_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants);
			if (SUCCEEDED(result))
			{
				if (errorMessages)
				{
					errorMessages->Release();
				}
			}
			else
			{
				if (errorMessages)
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName
						<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
					eae6320::UserOutput::Print(errorMessage.str());
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName;
					eae6320::UserOutput::Print(errorMessage.str());
				}
				return false;
			}
		}
		// Create the fragment shader object
		bool wereThereErrors = false;
		{
			HRESULT result = m_direct3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
				&m_fragmentShader);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}

	void Effect::SetEffect()
	{
		HRESULT result;
		result = m_direct3dDevice->SetVertexShader(m_vertexShader);
		assert(SUCCEEDED(result));

		result = m_direct3dDevice->SetPixelShader(m_fragmentShader);
		assert(SUCCEEDED(result));
	}

	bool Effect::CreateEffect(const char *i_vertexShaderFile, const char *i_fragmentShaderfile)
	{
		if (!LoadVertexShader(i_vertexShaderFile))
			return false;

		if (!LoadFragmentShader(i_fragmentShaderfile))
			return false;

		return true;
	}
}