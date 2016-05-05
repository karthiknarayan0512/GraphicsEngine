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
		void Render(Renderable *i_ConnectedPlayers);
		bool ShutDown();

		void MovePlayer(eae6320::Math::cVector &);

		Math::cVector GetPlayerPosition();

		void UpdateAllSpriteAtlases();

		Camera* getCamera();

		float UpdateStaminaMeter(bool bDecreaseStamina = true);

		void SetTeamRailing();

		void ResetFlag();

		void ResetEnemyFlag();

		bool IsEnemyFlagCarried();

		bool IsFlagCarried();

		void SetEnemyFlagLocation(Math::cVector &);

		Math::cVector GetFlagLocation();
		
		bool CheckScoreZoneProximity();

		int GetScore();

		void SetEnemyScore(int i_EnemyScore);

		bool ShouldSendScoreUpdate();
	}
}

#endif	// EAE6320_GRAPHICS_H
