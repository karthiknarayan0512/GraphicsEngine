// Header Files
//=============

#include "Graphics.h"
#include "../Networking/Networking.h"
#include "Context.h"
#include "Material.h"
#include "../../Engine/Physics/Physics.h"

#include "Player.h"

#ifdef _DEBUG
#include "DebugEffect.h"
#include "DebugLine.h"
#include "DebugSphere.h"
#include "DebugCylinder.h"
#include "DebugBox.h"
#include "DebugMenu.h"
#endif

#include "UIText.h"
#include "../Time/Time.h"

#include "Sprite.h"

#include <vector>

// Static Data Initialization
//===========================

namespace
{
	// User Controlled Camera
	eae6320::Graphics::Camera *s_Camera;

	bool s_TeamRailing;

	float currentStamina = 1.0f;

	bool FlagCarrying;

	int nTeamScore = 0;
	char *teamScore = new char[2];
	
	int nEnemyScore = 0;
	char *enemyScore = new char[2];

	bool m_ScoreUpdate;

	// Railing
	eae6320::Graphics::Renderable s_Railing;
	eae6320::Graphics::Renderable s_Metal;
	eae6320::Graphics::Renderable s_Ceiling;
	eae6320::Graphics::Renderable s_Cement;
	eae6320::Graphics::Renderable s_Walls;
	eae6320::Graphics::Renderable s_Floor;
	eae6320::Graphics::Renderable s_Boxes;

	// Flags
	eae6320::Graphics::Renderable *s_RedFlags = new eae6320::Graphics::Renderable[2];
	eae6320::Graphics::Renderable *s_BlueFlags = new eae6320::Graphics::Renderable[2];

	// Score Zones
	eae6320::Graphics::Renderable s_RedZone;
	eae6320::Graphics::Renderable s_BlueZone;

	// Player
	eae6320::Graphics::Renderable *s_Player = new eae6320::Graphics::Renderable[10];

	// Score
	eae6320::Graphics::UI::UIText s_FrameRate;
	eae6320::Graphics::UI::UIText s_PlayerScore;
	eae6320::Graphics::UI::UIText s_EnemyScore;

	// UI Sprites
	eae6320::Graphics::Sprite m_StaticSprite;
	eae6320::Graphics::Sprite m_DynamicSprite;
	eae6320::Graphics::Sprite m_StaminaSprite;

#ifdef _DEBUG
	eae6320::Graphics::DebugEffect s_DebugEffect;
	eae6320::Graphics::DebugShapes::DebugLine s_DebugLine[2];
	eae6320::Graphics::DebugShapes::DebugSphere s_DebugSphere[2];
	eae6320::Graphics::DebugShapes::DebugCylinder s_DebugCylinder[2];
	eae6320::Graphics::DebugShapes::DebugBox s_DebugBox[2];
#endif
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateRenderables();
	bool CreateCamera();
}

// Interface
//==========

int eae6320::Graphics::GetScore()
{
	return nTeamScore;
}

void eae6320::Graphics::SetEnemyScore(int i_EnemyScore)
{
	nEnemyScore = i_EnemyScore;
}

void eae6320::Graphics::ResetEnemyFlag()
{
	if (s_TeamRailing)
	{
		s_BlueFlags[1].m_position = eae6320::Math::cVector(1511.959, -69.069, 1.966);
	}
	else
	{
		s_RedFlags[1].m_position = eae6320::Math::cVector(-510.237, -69.700, -701.690);
	}
}

void eae6320::Graphics::ResetFlag()
{
	FlagCarrying = false;
}

eae6320::Graphics::Camera* eae6320::Graphics::getCamera()
{
	return s_Camera;
}

void eae6320::Graphics::SetTeamRailing()
{
	s_TeamRailing = true;
}

void eae6320::Graphics::MovePlayer(eae6320::Math::cVector &i_Position)
{
	i_Position.y -= 5.0f;
	Math::cVector Zero;
	Math::cMatrix_transformation nonTranslationMatrix(s_Camera->getCameraOrientation(), Zero);
	Math::cVector movementVector = i_Position * nonTranslationMatrix;

	Math::cVector end = s_Player[1].m_position + Math::cVector(movementVector.x, 0 - 10, 0);
	Graphics::Mesh::sVertex intersectionPoint;
	if (Physics::CheckCollisions(s_Player[1].m_position, end, intersectionPoint))
	{
		Math::cVector collisionNormal(intersectionPoint.nx, intersectionPoint.ny, intersectionPoint.nz);
		movementVector -= collisionNormal * Math::Dot(collisionNormal, movementVector);
	}

	end = s_Player[1].m_position + Math::cVector(0, movementVector.y - 10, 0);
	if (Physics::CheckCollisions(s_Player[1].m_position, end, intersectionPoint))
	{
		Math::cVector collisionNormal(intersectionPoint.nx, intersectionPoint.ny, intersectionPoint.nz);
		movementVector -= collisionNormal * Math::Dot(collisionNormal, movementVector);
	}

	end = s_Player[1].m_position + Math::cVector(0, 0 - 10, movementVector.z);
	if (Physics::CheckCollisions(s_Player[1].m_position, end, intersectionPoint))
	{
		Math::cVector collisionNormal(intersectionPoint.nx, intersectionPoint.ny, intersectionPoint.nz);
		movementVector -= collisionNormal * Math::Dot(collisionNormal, movementVector);
	}

	for (int i = 0; i < 11; i++)
	{
		s_Player[i].m_position += movementVector;
	}
}

bool eae6320::Graphics::CheckScoreZoneProximity()
{
	float length = (eae6320::Graphics::GetPlayerPosition() - (s_TeamRailing ? s_RedZone.m_position : s_BlueZone.m_position)).GetLength();

	return length < 200.0f;
}

bool CheckFlagProximity()
{
	float length = (eae6320::Graphics::GetPlayerPosition() - (s_TeamRailing ? s_RedFlags[0].m_position : s_BlueFlags[0].m_position)).GetLength();

	return length < 200.0f;
}

eae6320::Math::cVector eae6320::Graphics::GetPlayerPosition()
{
	return s_Player[0].m_position;
}

void eae6320::Graphics::UpdateAllSpriteAtlases()
{
	m_DynamicSprite.UpdateUVs(0.1f, 0.0f);
}

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	if (!Context::Initialize(i_renderingWindow))
		goto OnError;

	s_FrameRate.Create(eae6320::Time::GetFrameRate(), "Frame Rate");
	s_PlayerScore.Create(teamScore, "Score");
	s_EnemyScore.Create(enemyScore, "Score");
	eae6320::Graphics::DebugMenu::CreateDebugMenuFont();

	m_StaminaSprite.CreateTexture("data/Stamina.material", 0.9f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Create the renderables
	if (!CreateRenderables())
		goto OnError;

	if (!CreateCamera())
		goto OnError;

	return true;

OnError:

	ShutDown();
	return false;
}

float eae6320::Graphics::UpdateStaminaMeter(bool bDecreaseStamina)
{
	if (bDecreaseStamina)
	{
		currentStamina = currentStamina - Time::GetSecondsElapsedThisFrame();
		if (currentStamina < 0.0f)
			currentStamina = 0.0f;
	}
	else
	{
		currentStamina = currentStamina + (Time::GetSecondsElapsedThisFrame() / 3.0f);
		if (currentStamina > 1.0f)
			currentStamina = 1.0f;
	}

	m_StaminaSprite.UpdateTexture(0.9f, 1.0f, currentStamina, 0.0f);
	return currentStamina;
}

bool eae6320::Graphics::ShouldSendScoreUpdate()
{
	return m_ScoreUpdate;
}

void eae6320::Graphics::Render(Renderable *i_ConnectedPlayers)
{
	// Clear Buffers
	Context::ClearBuffers();
	{
		Context::BeginRender();
		{
			if (CheckFlagProximity() && !FlagCarrying)
			{
				FlagCarrying = true;
			}

			if (FlagCarrying)
			{
				if (s_TeamRailing)
				{
					s_RedFlags[1].m_position = GetPlayerPosition() + Math::cVector(0.0f, 30.0f, 0.0f);
				}
				else
				{
					s_BlueFlags[1].m_position = GetPlayerPosition() + Math::cVector(0.0f, 30.0f, 0.0f);
				}
				if (CheckScoreZoneProximity())
				{
					nTeamScore++;
					m_ScoreUpdate = true;
					ResetFlag();
					ResetEnemyFlag();
					Networking::TagBitch();
				}
			}
			else
			{
				if (s_TeamRailing)
				{
					s_RedFlags[1].m_position = eae6320::Math::cVector(-510.237, -69.700, -701.690);
				}
				else
				{
					s_BlueFlags[1].m_position = eae6320::Math::cVector(1511.959, -69.069, 1.966);
				}
			}

			RECT rect, scoreRect, enemyScoreRect;

			SetRect(&rect, 0, 0, 10, 10);
			s_FrameRate.Draw(&rect, 0, 255, 0);

			sprintf(teamScore, "%d", nTeamScore);
			SetRect(&scoreRect, 0, 20, 10, 10);
			s_PlayerScore.Draw(&scoreRect, 0, 255, 0);

			sprintf(enemyScore, "%d", nEnemyScore);
			SetRect(&enemyScoreRect, 700, 20, 10, 10);
			s_EnemyScore.Draw(&enemyScoreRect, 255, 0, 0);

			s_Railing.Render(*s_Camera);
			s_Ceiling.Render(*s_Camera);
			s_Cement.Render(*s_Camera);
			s_Metal.Render(*s_Camera);
			s_Floor.Render(*s_Camera);
			s_Walls.Render(*s_Camera);
			s_Boxes.Render(*s_Camera);

			s_RedZone.Render(*s_Camera);
			s_BlueZone.Render(*s_Camera);

			m_StaminaSprite.SetSprite(*s_Camera);

			for (int i = 0; i < 10; i++)
				s_Player[i].Render(*s_Camera);

			for (int i = 0; i < 2; i++)
			{
				s_RedFlags[i].Render(*s_Camera);
				s_BlueFlags[i].Render(*s_Camera);
			}

			if (i_ConnectedPlayers != NULL)
			{
				for (int i = 0; i < 10; i++)
					i_ConnectedPlayers[i].Render(*s_Camera);
			}
		}
		Context::EndRender();
	}

	Context::SwapBuffers();
}

bool eae6320::Graphics::IsFlagCarried()
{
	return FlagCarrying;
}

bool eae6320::Graphics::IsEnemyFlagCarried()
{
	Renderable enemyFlag;
	Math::cVector enemyFlagPosition;

	enemyFlag = s_TeamRailing ? s_BlueFlags[1] : s_RedFlags[1];
	enemyFlagPosition = s_TeamRailing ? eae6320::Math::cVector(1511.959, -69.069, 1.966) : eae6320::Math::cVector(-510.237, -69.700, -701.690);

	return enemyFlag.m_position != enemyFlagPosition;
}

eae6320::Math::cVector eae6320::Graphics::GetFlagLocation()
{
	return s_TeamRailing ? s_RedFlags[1].m_position : s_BlueFlags[1].m_position;
}

bool eae6320::Graphics::ShutDown()
{
	return Context::Shutdown();
}

void eae6320::Graphics::SetEnemyFlagLocation(Math::cVector & i_EnemyFlagLocation)
{
	if (s_TeamRailing)
		s_BlueFlags[1].m_position = i_EnemyFlagLocation;
	else
		s_RedFlags[1].m_position = i_EnemyFlagLocation;
}
// Helper Function Definitions
//============================

namespace
{
	bool CreateRenderables()
	{
		eae6320::Physics::Initialize("data/lambert3.mesh");

		// Create Railing
		s_Railing.m_Mesh.LoadMeshFromFile("data/railing_m.mesh");
		s_Railing.m_Material.LoadMaterial("data/railing_m.material");

		s_Ceiling.m_Mesh.LoadMeshFromFile("data/ceiling_m.mesh");
		s_Ceiling.m_Material.LoadMaterial("data/ceiling_m.material");

		s_Cement.m_Mesh.LoadMeshFromFile("data/cement_m.mesh");
		s_Cement.m_Material.LoadMaterial("data/cement_m.material");

		s_Metal.m_Mesh.LoadMeshFromFile("data/metal_m.mesh");
		s_Metal.m_Material.LoadMaterial("data/metal_m.material");

		s_Floor.m_Mesh.LoadMeshFromFile("data/floor_m.mesh");
		s_Floor.m_Material.LoadMaterial("data/floor_m.material");

		s_Walls.m_Mesh.LoadMeshFromFile("data/walls_m.mesh");
		s_Walls.m_Material.LoadMaterial("data/walls_m.material");

		s_Boxes.m_Mesh.LoadMeshFromFile("data/lambert2.mesh");
		s_Boxes.m_Material.LoadMaterial("data/lambert2.material");

		for (int i = 0; i < 10; i++)
		{
			char path[100];
			sprintf_s(path, "data/phong%d.mesh", i + 1);
			s_Player[i].m_Mesh.LoadMeshFromFile(path);

			sprintf_s(path, "data/phong%d.material", i + 1);
			s_Player[i].m_Material.LoadMaterial(path);

			s_Player[i].m_position += eae6320::Math::cVector(0.0f, -200.0f, s_TeamRailing ? 0.0f : -400.0f);
		}

		// Create Flags
		s_RedFlags[0].m_Mesh.LoadMeshFromFile("data/FlagPole.mesh");
		s_RedFlags[0].m_Material.LoadMaterial("data/FlagPole.material");
		s_RedFlags[0].m_position = eae6320::Math::cVector(-510.512, -102.146, -672.390);

		s_RedFlags[1].m_Mesh.LoadMeshFromFile("data/RailingFlag.mesh");
		s_RedFlags[1].m_Material.LoadMaterial("data/RailingFlag.material");
		s_RedFlags[1].m_position = eae6320::Math::cVector(-510.237, -69.700, -701.690);

		s_BlueFlags[0].m_Mesh.LoadMeshFromFile("data/FlagPole.mesh");
		s_BlueFlags[0].m_Material.LoadMaterial("data/FlagPole.material");
		s_BlueFlags[0].m_position = eae6320::Math::cVector(1511.684, -101.515, 31.266);

		s_BlueFlags[1].m_Mesh.LoadMeshFromFile("data/BrownFlag.mesh");
		s_BlueFlags[1].m_Material.LoadMaterial("data/BrownFlag.material");
		s_BlueFlags[1].m_position = eae6320::Math::cVector(1511.959, -69.069, 1.966);

		// Create Score Zones
		s_RedZone.m_Mesh.LoadMeshFromFile("data/RailingPlatform.mesh");
		s_RedZone.m_Material.LoadMaterial("data/RailingPlatform.material");
		s_RedZone.m_position = eae6320::Math::cVector(264.878, -248.062, -1248.545);

		s_BlueZone.m_Mesh.LoadMeshFromFile("data/BrownPlatform.mesh");
		s_BlueZone.m_Material.LoadMaterial("data/BrownPlatform.material");
		s_BlueZone.m_position = eae6320::Math::cVector(-389.561, -239.811, 1397.616);

		return true;
	}

	bool CreateCamera()
	{
		s_Camera = new eae6320::Graphics::Camera(0, 0, 0);
		return s_Camera != NULL;
	}
}
