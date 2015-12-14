// Header Files
//=============

#include "Graphics.h"
#include "Context.h"
#include "Material.h"

#include <vector>

// Static Data Initialization
//===========================

namespace
{
	// User Controlled Camera
	eae6320::Graphics::Camera *s_Camera;

	// Player
	eae6320::Graphics::Renderable s_Player;

	// Collectibles
	eae6320::Graphics::Renderable s_Pig[6];
	eae6320::Graphics::Renderable s_Coconut[6];
	eae6320::Graphics::Renderable s_Banana[6];

	// Main Cube
	eae6320::Graphics::Renderable s_Cube;

	size_t currentFace = 0;

	bool bPigCollected;
	bool bCoconutCollected;
	bool bBananaCollected;

	bool bCubeRotating;
	float angleRotated = 0.0f;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateRenderables();
	bool CreateCamera();
	bool CollisionCheck(eae6320::Math::cVector i_firstObject, eae6320::Math::cVector i_secondObject, float distanceCheck);
}

// Interface
//==========

void eae6320::Graphics::movePlayer(float i_offsetX, float i_offsetY)
{
	s_Player.m_position.x += i_offsetX;
	s_Player.m_position.y += i_offsetY;
}

eae6320::Graphics::Camera* eae6320::Graphics::getCamera()
{
	return s_Camera;
}

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	if (!Context::Initialize(i_renderingWindow))
		goto OnError;

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

void RotateCube()
{
	switch (currentFace)
	{
	case 0:
	case 1:
	case 2:
		angleRotated += 0.0174533f;
		if (angleRotated < 1.5708f)
		{
			s_Cube.m_orientation = eae6320::Math::cQuaternion(angleRotated, eae6320::Math::cVector(0.0f, 1.0f, 0.0f));
			bCubeRotating = true;
		}
		else
			bCubeRotating = false;
		break;
	case 3:
		angleRotated += 0.0174533f;
		if (angleRotated < 1.5708f)
		{
			s_Cube.m_orientation = eae6320::Math::cQuaternion(angleRotated, eae6320::Math::cVector(1.0f, 0.0f, 0.0f));
			bCubeRotating = true;
		}
		else
			bCubeRotating = false;
		break;
	case 4:
		angleRotated += 0.0174533f;
		if (angleRotated < 3.1416f)
		{
			s_Cube.m_orientation = eae6320::Math::cQuaternion(angleRotated, eae6320::Math::cVector(1.0f, 0.0f, 0.0f));
			bCubeRotating = true;
		}
		else
			bCubeRotating = false;
		break;
	default:
		break;
	}
}

void eae6320::Graphics::Render()
{
	// Clear Buffers
	Context::ClearBuffers();

	{
		Context::BeginRender();
		{
			s_Cube.Render(*s_Camera);
			if (!bCubeRotating)
			{
				s_Player.m_position.z = 3.0f;
				s_Player.Render(*s_Camera);

				switch (currentFace)
				{
				case 0:
					if (!bPigCollected)
					{
						s_Pig[currentFace].m_position.y = 2.3f;
						s_Pig[currentFace].m_position.x = -2.2f;
						s_Pig[currentFace].Render(*s_Camera);
						bPigCollected = CollisionCheck(s_Pig[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bCoconutCollected)
					{
						s_Coconut[currentFace].m_position.x = 0.0f;
						s_Coconut[currentFace].m_position.y = -2.3f;
						s_Coconut[currentFace].Render(*s_Camera);
						bCoconutCollected = CollisionCheck(s_Coconut[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bBananaCollected)
					{
						s_Banana[currentFace].m_position.x = 2.3f;
						s_Banana[currentFace].m_position.y = 1.8f;
						s_Banana[currentFace].Render(*s_Camera);
						bBananaCollected = CollisionCheck(s_Banana[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					break;
				case 1:
					if (!bPigCollected)
					{
						s_Pig[currentFace].m_position.y = 2.3f;
						s_Pig[currentFace].m_position.x = 2.2f;
						s_Pig[currentFace].Render(*s_Camera);
						bPigCollected = CollisionCheck(s_Pig[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bCoconutCollected)
					{
						s_Coconut[currentFace].m_position.x = -2.5f;
						s_Coconut[currentFace].m_position.y = 0.0f;
						s_Coconut[currentFace].Render(*s_Camera);
						bCoconutCollected = CollisionCheck(s_Coconut[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bBananaCollected)
					{
						s_Banana[currentFace].m_position.x = 2.3f;
						s_Banana[currentFace].m_position.y = -2.5f;
						s_Banana[currentFace].Render(*s_Camera);
						bBananaCollected = CollisionCheck(s_Banana[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					break;
				case 2:
					if (!bPigCollected)
					{
						s_Pig[currentFace].m_position.y = 2.3f;
						s_Pig[currentFace].m_position.x = 0.0f;
						s_Pig[currentFace].Render(*s_Camera);
						bPigCollected = CollisionCheck(s_Pig[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bCoconutCollected)
					{
						s_Coconut[currentFace].m_position.x = 0.0f;
						s_Coconut[currentFace].m_position.y = 0.0f;
						s_Coconut[currentFace].Render(*s_Camera);
						bCoconutCollected = CollisionCheck(s_Coconut[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bBananaCollected)
					{
						s_Banana[currentFace].m_position.x = 0.0f;
						s_Banana[currentFace].m_position.y = -2.5f;
						s_Banana[currentFace].Render(*s_Camera);
						bBananaCollected = CollisionCheck(s_Banana[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					break;
				case 3:
					if (!bPigCollected)
					{
						s_Pig[currentFace].m_position.y = 0.0f;
						s_Pig[currentFace].m_position.x = 2.2f;
						s_Pig[currentFace].Render(*s_Camera);
						bPigCollected = CollisionCheck(s_Pig[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bCoconutCollected)
					{
						s_Coconut[currentFace].m_position.x = -2.5f;
						s_Coconut[currentFace].m_position.y = 0.0f;
						s_Coconut[currentFace].Render(*s_Camera);
						bCoconutCollected = CollisionCheck(s_Coconut[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bBananaCollected)
					{
						s_Banana[currentFace].m_position.x = 0.0f;
						s_Banana[currentFace].m_position.y = 0.0f;
						s_Banana[currentFace].Render(*s_Camera);
						bBananaCollected = CollisionCheck(s_Banana[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					break;
				case 4:
					if (!bPigCollected)
					{
						s_Pig[currentFace].m_position.y = 2.3f;
						s_Pig[currentFace].m_position.x = -2.2f;
						s_Pig[currentFace].Render(*s_Camera);
						bPigCollected = CollisionCheck(s_Pig[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bCoconutCollected)
					{
						s_Coconut[currentFace].m_position.x = 0.0f;
						s_Coconut[currentFace].m_position.y = 0.0f;
						s_Coconut[currentFace].Render(*s_Camera);
						bCoconutCollected = CollisionCheck(s_Coconut[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bBananaCollected)
					{
						s_Banana[currentFace].m_position.x = 2.3f;
						s_Banana[currentFace].m_position.y = -2.5f;
						s_Banana[currentFace].Render(*s_Camera);
						bBananaCollected = CollisionCheck(s_Banana[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					break;
				case 5:
					if (!bPigCollected)
					{
						s_Pig[currentFace].m_position.y = 2.3f;
						s_Pig[currentFace].m_position.x = 2.2f;
						s_Pig[currentFace].Render(*s_Camera);
						bPigCollected = CollisionCheck(s_Pig[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bCoconutCollected)
					{
						s_Coconut[currentFace].m_position.x = 0.0f;
						s_Coconut[currentFace].m_position.y = 0.0f;
						s_Coconut[currentFace].Render(*s_Camera);
						bCoconutCollected = CollisionCheck(s_Coconut[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					if (!bBananaCollected)
					{
						s_Banana[currentFace].m_position.x = -2.3f;
						s_Banana[currentFace].m_position.y = -2.5f;
						s_Banana[currentFace].Render(*s_Camera);
						bBananaCollected = CollisionCheck(s_Banana[currentFace].m_position, s_Player.m_position, 1.0f);
					}
					break;
				}
			}

			if (bPigCollected && bCoconutCollected && bBananaCollected)
			{
				RotateCube();
				if (!bCubeRotating)
				{
					currentFace++;
					bPigCollected = false;
					bBananaCollected = false;
					bCoconutCollected = false;
					angleRotated = 0.0f;
				}
			}
		}
		Context::EndRender();
	}

	Context::SwapBuffers();
}

bool eae6320::Graphics::ShutDown()
{
	return Context::Shutdown();
}
// Helper Function Definitions
//============================

namespace
{
	bool CreateRenderables()
	{
		// Create Player
		s_Player.m_Mesh.LoadMeshFromFile("data/Player.lua");
		s_Player.m_Material.LoadMaterial("data/PlayerMaterial.lua");

		// Create taro collectibles
		for (int i = 0; i < 6; i++)
		{
			s_Pig[i].m_Mesh.LoadMeshFromFile("data/Pig.lua");
			s_Pig[i].m_Material.LoadMaterial("data/PigMaterial.lua");
			s_Pig[i].m_position.z = 3.0f;
		}

		// Create coconut collectibles
		for (int i = 0; i < 6; i++)
		{
			s_Coconut[i].m_Mesh.LoadMeshFromFile("data/Coconut.lua");
			s_Coconut[i].m_Material.LoadMaterial("data/CoconutMaterial.lua");
			s_Coconut[i].m_position.z = 3.0f;
		}

		// Create coconut collectibles
		for (int i = 0; i < 6; i++)
		{
			s_Banana[i].m_Mesh.LoadMeshFromFile("data/Banana.lua");
			s_Banana[i].m_Material.LoadMaterial("data/BananaMaterial.lua");
			s_Banana[i].m_position.z = 3.5f;
		}

		// Create main cube
		s_Cube.m_Mesh.LoadMeshFromFile("data/Cube.lua");
		s_Cube.m_Material.LoadMaterial("data/CubeMaterial.lua");

		return true;
	}

	bool CollisionCheck(eae6320::Math::cVector i_firstObject, eae6320::Math::cVector i_secondObject, float distanceCheck)
	{
		if ((i_firstObject - i_secondObject).GetLength() < distanceCheck)
			return true;

		return false;
	}

	bool CreateCamera()
	{
		s_Camera = new eae6320::Graphics::Camera(0, 0, 10);
		return s_Camera != NULL;
	}
}
