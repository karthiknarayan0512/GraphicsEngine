// Header Files
//=============

#include "Graphics.h"
#include "Context.h"

// Static Data Initialization
//===========================

namespace
{
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
}

// Interface
//==========

eae6320::Graphics::Renderable* eae6320::Graphics::getUserControlledRenderables(int &i_length)
{
	i_length = 1;
	return s_UserControlledObjects;
}

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	if (!Context::Initialize(i_renderingWindow))
		goto OnError;

	// Create the renderables
	if (!CreateRenderables())
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
				s_UserControlledObjects[i].Render();
			for (int i = 0; i < 2; i++)
				s_StaticObjects[i].Render();
		}
		Context::EndRender();
	}

	Context::SwapBuffers();
}

bool eae6320::Graphics::ShutDown()
{
	delete s_StaticObjects;

	delete s_UserControlledObjects;

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
			if (!s_UserControlledObjects[i].m_Mesh.LoadMeshFromFile("data/Mesh.lua"))
				return false;
			if (!s_UserControlledObjects[i].m_Effect.CreateEffect("data/Effect.lua"))
				return false;
		}

		s_StaticObjects = new eae6320::Graphics::Renderable[2];
		for (int i = 0; i < 2; i++)
		{
			if (!s_StaticObjects[i].m_Mesh.LoadMeshFromFile("data/Triangle.lua"))
				return false;
			if (!s_StaticObjects[i].m_Effect.CreateEffect("data/Effect.lua"))
				return false;
		}
		s_StaticObjects[0].m_positionOffset.x = -0.75;
		s_StaticObjects[0].m_positionOffset.y = 0.75;

		s_StaticObjects[1].m_positionOffset.x = 0.75;
		s_StaticObjects[1].m_positionOffset.y = -0.75;

		return true;
	}
}
