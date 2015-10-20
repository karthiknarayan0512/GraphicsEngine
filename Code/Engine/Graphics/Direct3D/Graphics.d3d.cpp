// Header Files
//=============

#include "../Graphics.h"

#include <cassert>
#include <cstdint>
#include <d3dx9shader.h>
#include <sstream>
#include "../../UserOutput/UserOutput.h"

// Static Data Initialization
//===========================

namespace
{
	HWND s_renderingWindow = NULL;
	IDirect3D9* s_direct3dInterface = NULL;
	IDirect3DDevice9* s_direct3dDevice = NULL;

	// A list of input controlled renderables;
	eae6320::Renderable *s_UserControlledObjects;

	// A list of static renderables;
	eae6320::Renderable *s_StaticObjects;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateDevice();
	bool CreateInterface();
	bool CreateRenderables();
}

// Interface
//==========

IDirect3DDevice9* eae6320::Graphics::getDirect3DDevice()
{
	return s_direct3dDevice;
}

eae6320::Renderable* eae6320::Graphics::getUserControlledRenderables(int &i_length)
{
	i_length = 1;
	return s_UserControlledObjects;
}

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	s_renderingWindow = i_renderingWindow;

	// Initialize the interface to the Direct3D9 library
	if ( !CreateInterface() )
	{
		return false;
	}
	// Create an interface to a Direct3D device
	if ( !CreateDevice() )
	{
		goto OnError;
	}

	// Create the mesh
	if (!CreateRenderables())
	{
		goto OnError;
	}

	return true;

OnError:

	ShutDown();
	return false;
}

void eae6320::Graphics::Render()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		const D3DRECT* subRectanglesToClear = NULL;
		const DWORD subRectangleCount = 0;
		const DWORD clearTheRenderTarget = D3DCLEAR_TARGET;
		D3DCOLOR clearColor;
		{
			// Black is usually used:
			clearColor = D3DCOLOR_XRGB( 0, 0, 0 );
		}
		const float noZBuffer = 0.0f;
		const DWORD noStencilBuffer = 0;
		HRESULT result = s_direct3dDevice->Clear( subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, clearColor, noZBuffer, noStencilBuffer );
		assert( SUCCEEDED( result ) );
	}

	// The actual function calls that draw geometry must be made between paired calls to
	// BeginScene() and EndScene()
	{
		HRESULT result = s_direct3dDevice->BeginScene();
		assert( SUCCEEDED( result ) );
		{
			for (int i = 0; i < 1; i++)
				s_UserControlledObjects[i].Render();
			for (int i = 0; i < 2; i++)
				s_StaticObjects[i].Render();
		}
		result = s_direct3dDevice->EndScene();
		assert( SUCCEEDED( result ) );
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be "presented"
	// (to the front buffer)
	{
		const RECT* noSourceRectangle = NULL;
		const RECT* noDestinationRectangle = NULL;
		const HWND useDefaultWindow = NULL;
		const RGNDATA* noDirtyRegion = NULL;
		HRESULT result = s_direct3dDevice->Present( noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion );
		assert( SUCCEEDED( result ) );
	}
}

bool eae6320::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if ( s_direct3dInterface )
	{
		if ( s_direct3dDevice )
		{
			s_direct3dDevice->Release();
			s_direct3dDevice = NULL;
		}

		s_direct3dInterface->Release();
		s_direct3dInterface = NULL;
	}
	s_renderingWindow = NULL;

	return !wereThereErrors;
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateDevice()
	{
		const UINT useDefaultDevice = D3DADAPTER_DEFAULT;
		const D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
		const DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		D3DPRESENT_PARAMETERS presentationParameters = { 0 };
		{
			{
				const unsigned int useRenderingWindowDimensions = 0;
				presentationParameters.BackBufferWidth = useRenderingWindowDimensions;
				presentationParameters.BackBufferHeight = useRenderingWindowDimensions;
			}
			presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
			presentationParameters.BackBufferCount = 1;
			presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentationParameters.hDeviceWindow = s_renderingWindow;
			presentationParameters.Windowed = TRUE;
			presentationParameters.EnableAutoDepthStencil = FALSE;
			presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		HRESULT result = s_direct3dInterface->CreateDevice( useDefaultDevice, useHardwareRendering,
			s_renderingWindow, useHardwareVertexProcessing, &presentationParameters, &s_direct3dDevice );
		if ( SUCCEEDED( result ) )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "Direct3D failed to create a Direct3D9 device" );
			return false;
		}
	}

	bool CreateInterface()
	{
		// D3D_SDK_VERSION is #defined by the Direct3D header files,
		// and is just a way to make sure that everything is up-to-date
		s_direct3dInterface = Direct3DCreate9( D3D_SDK_VERSION );
		if ( s_direct3dInterface )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "DirectX failed to create a Direct3D9 interface" );
			return false;
		}
	}
	
	bool CreateRenderables()
	{
		s_UserControlledObjects = new eae6320::Renderable[1];
		for (int i = 0; i < 1; i++)
		{
			if (!s_UserControlledObjects[i].m_Mesh.LoadMeshFromFile("data/Mesh.lua"))
				return false;
			if (!s_UserControlledObjects[i].m_Effect.CreateEffect("data/vertex.shader", "data/fragment.shader"))
				return false;
		}

		s_StaticObjects = new eae6320::Renderable[2];
		for (int i = 0; i < 2; i++)
		{
			if (!s_StaticObjects[i].m_Mesh.LoadMeshFromFile("data/Triangle.lua"))
				return false;
			if (!s_StaticObjects[i].m_Effect.CreateEffect("data/vertex.shader", "data/fragment.shader"))
				return false;
		}
		s_StaticObjects[0].m_positionOffset.x = -0.75;
		s_StaticObjects[0].m_positionOffset.y = 0.75;

		s_StaticObjects[1].m_positionOffset.x = 0.75;
		s_StaticObjects[1].m_positionOffset.y = -0.75;

		return true;
	}
}
