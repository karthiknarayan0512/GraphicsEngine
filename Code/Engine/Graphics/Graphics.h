/*
	This file contains the function declarations for graphics
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Header Files
//=============

#include "../Windows/Includes.h"
#include "Renderable.h"
#include "Camera.h"
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

		void MovePlayer(eae6320::Math::cVector &);

		Math::cVector GetPlayerPosition();

		void UpdateAllSpriteAtlases();

		Camera* getCamera();
	}
}

#endif	// EAE6320_GRAPHICS_H
