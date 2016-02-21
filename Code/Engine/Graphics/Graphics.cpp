// Header Files
//=============

#include "Graphics.h"
#include "Context.h"
#include "Material.h"

#ifdef _DEBUG
#include "DebugEffect.h"
#include "DebugLine.h"
#include "DebugSphere.h"
#include "DebugCylinder.h"
#include "DebugBox.h"
#endif

#include "Sprite.h"

#include <vector>

// Static Data Initialization
//===========================

namespace
{
	// User Controlled Camera
	eae6320::Graphics::Camera *s_Camera;

	// Railing
	eae6320::Graphics::Renderable s_Railing;
	eae6320::Graphics::Renderable s_Metal;
	eae6320::Graphics::Renderable s_Ceiling;
	eae6320::Graphics::Renderable s_Cement;
	eae6320::Graphics::Renderable s_Walls;
	eae6320::Graphics::Renderable s_Floor;

	// UI Sprites
	eae6320::Graphics::Sprite m_StaticSprite;
	eae6320::Graphics::Sprite m_DynamicSprite;

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

eae6320::Graphics::Camera* eae6320::Graphics::getCamera()
{
	return s_Camera;
}

void eae6320::Graphics::UpdateAllSpriteAtlases()
{
	m_DynamicSprite.UpdateUVs(0.1f, 0.0f);
}

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	if (!Context::Initialize(i_renderingWindow))
		goto OnError;

#ifdef _DEBUG
	s_DebugEffect.CreateEffect("data/debugEffect.lua");
	D3DVECTOR startPoint[2];
	startPoint[0].x = -5.0f;
	startPoint[0].y = 0.0f;
	startPoint[0].z = 0.0f;

	startPoint[1].x = -5.0f;
	startPoint[1].y = 5.0f;
	startPoint[1].z = 0.0f;

	D3DVECTOR endPoint[2];
	endPoint[0].x = 5.0f;
	endPoint[0].y = 0.0f;
	endPoint[0].z = 0.0f;

	endPoint[1].x = 5.0f;
	endPoint[1].y = 5.0f;
	endPoint[1].z = 0.0f;


	// Create Debug lines
	s_DebugLine[0].CreateLine(startPoint[0], endPoint[0], 0, 0, 255);
	s_DebugLine[1].CreateLine(startPoint[1], endPoint[1], 255, 0, 0);

	{
		D3DVECTOR sphereOrigin[2];
		sphereOrigin[0].x = -5.0f;
		sphereOrigin[0].y = 2.5f;
		sphereOrigin[0].z = 0.0f;

		sphereOrigin[1].x = 5.0f;
		sphereOrigin[1].y = 2.5f;
		sphereOrigin[1].z = 0.0f;

		s_DebugSphere[0].CreateSphere(1.0f, 20, 20, 255, 0, 0, sphereOrigin[0]);
		s_DebugSphere[1].CreateSphere(1.0f, 20, 20, 0, 255, 0, sphereOrigin[1]);
	}

	{
		D3DVECTOR cylinderOrigin[2];
		cylinderOrigin[0].x = -3.0f;
		cylinderOrigin[0].y = 2.5f;
		cylinderOrigin[0].z = 0.0f;

		cylinderOrigin[1].x = 3.0f;
		cylinderOrigin[1].y = 2.5f;
		cylinderOrigin[1].z = 0.0f;

		s_DebugCylinder[0].CreateCylinder(1.0f, 1.0f, 5.0f, 20, 20, 255, 255, 0, cylinderOrigin[0]);
		s_DebugCylinder[1].CreateCylinder(1.0f, 1.0f, 5.0f, 20, 20, 255, 0, 255, cylinderOrigin[1]);
	}

	{
		D3DVECTOR boxOrigin[2];
		boxOrigin[0].x = -1.0f;
		boxOrigin[0].y = 0.5f;
		boxOrigin[0].z = 0.0f;

		boxOrigin[1].x = -1.0f;
		boxOrigin[1].y = 2.5f;
		boxOrigin[1].z = 0.0f;

		s_DebugBox[0].CreateBox(1.0f, 1.0f, 1.0f, 125, 0, 0, boxOrigin[0]);
		s_DebugBox[1].CreateBox(1.0f, 1.0f, 1.0f, 15, 125, 147, boxOrigin[1]);
	}
#endif

	m_StaticSprite.CreateTexture("data/texture.material", 0.5f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f);
	m_DynamicSprite.CreateTexture("data/numbers.material", 0.6f, 0.8f, -0.5f, -0.9f, 0.1f, 1.0f);

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

void eae6320::Graphics::Render()
{
	// Clear Buffers
	Context::ClearBuffers();

	{
		Context::BeginRender();
		{
			s_Railing.Render(*s_Camera);
			s_Ceiling.Render(*s_Camera);
			s_Cement.Render(*s_Camera);
			s_Metal.Render(*s_Camera);
			s_Floor.Render(*s_Camera);
			s_Walls.Render(*s_Camera);

			m_StaticSprite.SetSprite(*s_Camera);
			m_DynamicSprite.SetSprite(*s_Camera);

#ifdef _DEBUG
			//s_DebugLine[0].AddLine();
			//s_DebugLine[1].AddLine();
			//s_DebugSphere[0].DrawSphere();
			//s_DebugSphere[1].DrawSphere();
			//s_DebugCylinder[0].DrawCylinder();
			//s_DebugCylinder[1].DrawCylinder();
			//s_DebugBox[0].DrawBox();
			//s_DebugBox[1].DrawBox();
#endif

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
		// Create Railing
		s_Railing.m_Mesh.LoadMeshFromFile("data/railing_m.mesh");
		s_Railing.m_Material.LoadMaterial("data/railing.material");

		s_Ceiling.m_Mesh.LoadMeshFromFile("data/ceiling_m.mesh");
		s_Ceiling.m_Material.LoadMaterial("data/cement.material");

		s_Cement.m_Mesh.LoadMeshFromFile("data/cement_m.mesh");
		s_Cement.m_Material.LoadMaterial("data/cement.material");

		s_Metal.m_Mesh.LoadMeshFromFile("data/metal_m.mesh");
		s_Metal.m_Material.LoadMaterial("data/metal.material");

		s_Floor.m_Mesh.LoadMeshFromFile("data/floor_m.mesh");
		s_Floor.m_Material.LoadMaterial("data/floor.material");

		s_Walls.m_Mesh.LoadMeshFromFile("data/walls_m.mesh");
		s_Walls.m_Material.LoadMaterial("data/wall.material");

		return true;
	}

	bool CreateCamera()
	{
		s_Camera = new eae6320::Graphics::Camera(0, 0, 10);
		return s_Camera != NULL;
	}
}
