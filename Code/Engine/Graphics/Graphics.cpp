// Header Files
//=============

#include "Graphics.h"
#include "Context.h"

// Static Data Initialization
//===========================

namespace
{
	// User Controlled Camera
	eae6320::Graphics::Camera *s_Camera;

	// A list of input controlled renderables;
	eae6320::Graphics::Renderable *s_UserControlledObjects;

	// A list of static renderables;
	eae6320::Graphics::Renderable *s_StaticObjects;
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

eae6320::Graphics::Renderable* eae6320::Graphics::getUserControlledRenderables(int &i_length)
{
	i_length = 1;
	return s_UserControlledObjects;
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

void eae6320::Graphics::Render()
{
	// Clear Buffers
	Context::ClearBuffers();

	{
		Context::BeginRender();
		{
			for (int i = 0; i < 1; i++)
				s_UserControlledObjects[i].Render(*s_Camera);
			for (int i = 0; i < 1; i++)
				s_StaticObjects[i].Render(*s_Camera);
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
		s_UserControlledObjects = new eae6320::Graphics::Renderable[1];
		for (int i = 0; i < 1; i++)
		{
			if (!s_UserControlledObjects[i].m_Mesh.LoadMeshFromFile("data/Box.lua"))
				return false;
			if (!s_UserControlledObjects[i].m_Effect.CreateEffect("data/Effect.lua"))
				return false;
		}

		s_StaticObjects = new eae6320::Graphics::Renderable[1];
		for (int i = 0; i < 1; i++)
		{
			if (!s_StaticObjects[i].m_Mesh.LoadMeshFromFile("data/Floor.lua"))
				return false;
			if (!s_StaticObjects[i].m_Effect.CreateEffect("data/Effect.lua"))
				return false;
		}

		return true;
	}

	bool CreateCamera()
	{
		s_Camera = new eae6320::Graphics::Camera();
		return s_Camera != NULL;
	}
}
