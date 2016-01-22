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

	// Railing
	eae6320::Graphics::Renderable s_Railing;
	eae6320::Graphics::Renderable s_Metal;
	eae6320::Graphics::Renderable s_Ceiling;
	eae6320::Graphics::Renderable s_Cement;
	eae6320::Graphics::Renderable s_Walls;
	eae6320::Graphics::Renderable s_Floor;
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
