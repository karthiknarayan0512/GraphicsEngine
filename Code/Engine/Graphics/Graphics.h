/*
	This file contains the function declarations for graphics
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Header Files
//=============

#include "../Windows/Includes.h"
#include "Renderable.h"
#ifdef EAE6320_PLATFORM_D3D
#include <d3d9.h>
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		bool Initialize( const HWND i_renderingWindow );
		void Render();
		bool ShutDown();

		Renderable* getUserControlledRenderables(int &i_length);
#ifdef EAE6320_PLATFORM_D3D
		IDirect3DDevice9* getDirect3DDevice();
#endif
	}
}

#endif	// EAE6320_GRAPHICS_H
