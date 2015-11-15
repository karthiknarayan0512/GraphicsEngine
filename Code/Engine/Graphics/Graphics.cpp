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

	// A list of static opaque renderables;
	eae6320::Graphics::Renderable *s_StaticOpaqueObjects;

	// A list of static transparent renderables;
	eae6320::Graphics::Renderable *s_StaticTransparentObjects;
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
			for (int i = 0; i < 2; i++)
				s_StaticOpaqueObjects[i].Render(*s_Camera);
			for (int i = 0; i < 1; i++)
				s_StaticTransparentObjects[i].Render(*s_Camera);
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
			if (!s_UserControlledObjects[i].m_Mesh.LoadMeshFromFile("data/Football.lua"))
				return false;
			if (!s_UserControlledObjects[i].m_Effect.CreateEffect("data/Effect.lua"))
				return false;
			s_UserControlledObjects[i].m_position.z = -2.0f;
			s_UserControlledObjects[i].m_position.x = -1.0f;
		}

		s_StaticOpaqueObjects = new eae6320::Graphics::Renderable[2];
		{
			if (!s_StaticOpaqueObjects[0].m_Mesh.LoadMeshFromFile("data/Floor.lua"))
				return false;
			if (!s_StaticOpaqueObjects[0].m_Effect.CreateEffect("data/Effect.lua"))
				return false;
			if (!s_StaticOpaqueObjects[1].m_Mesh.LoadMeshFromFile("data/Dodec.lua"))
				return false;
			if (!s_StaticOpaqueObjects[1].m_Effect.CreateEffect("data/Effect.lua"))
				return false;
			s_StaticOpaqueObjects[1].m_position.z = 2.0f;
			s_StaticOpaqueObjects[1].m_position.x = 2.0f;
		}

		s_StaticTransparentObjects = new eae6320::Graphics::Renderable[1];
		for (int i = 0; i < 1; i++)
		{
			if (!s_StaticTransparentObjects[i].m_Mesh.LoadMeshFromFile("data/Pipe.lua"))
				return false;
			if (!s_StaticTransparentObjects[i].m_Effect.CreateEffect("data/TransparentEffect.lua"))
				return false;
		}

		return true;
	}

	bool CreateCamera()
	{
		s_Camera = new eae6320::Graphics::Camera(0, 0, 10);
		return s_Camera != NULL;
	}
}
