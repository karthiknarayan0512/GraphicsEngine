// Header Files
//=============

#include "Graphics.h"
#include "Context.h"
#include "Material.h"

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

	eae6320::Graphics::Material testMaterial;
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
			for (int i = 0; i < 4; i++)
				s_StaticOpaqueObjects[i].Render(*s_Camera);
			for (int i = 0; i < 2; i++)
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
			s_UserControlledObjects[i].m_Material.LoadMaterial("data/OpaqueShaderMaterial.lua");
				
			s_UserControlledObjects[i].m_position.z = -2.0f;
			s_UserControlledObjects[i].m_position.y = 3.0f;
			s_UserControlledObjects[i].m_position.x = -1.0f;
		}

		s_StaticOpaqueObjects = new eae6320::Graphics::Renderable[4];
		{
			if (!s_StaticOpaqueObjects[0].m_Mesh.LoadMeshFromFile("data/Floor.lua"))
				return false;
			s_StaticOpaqueObjects[0].m_Material.LoadMaterial("data/OpaqueShaderMaterial.lua");
				
			if (!s_StaticOpaqueObjects[1].m_Mesh.LoadMeshFromFile("data/Dodec.lua"))
				return false;
			s_StaticOpaqueObjects[1].m_Material.LoadMaterial("data/OpaqueShaderMaterial.lua");
				
			s_StaticOpaqueObjects[1].m_position.z = 2.0f;
			s_StaticOpaqueObjects[1].m_position.x = 1.5f;
			s_StaticOpaqueObjects[1].m_position.y = 3.0f;

			if (!s_StaticOpaqueObjects[2].m_Mesh.LoadMeshFromFile("data/Pipe.lua"))
				return false;
			s_StaticOpaqueObjects[2].m_Material.LoadMaterial("data/OpaqueShaderMaterial_blue.lua");

			s_StaticOpaqueObjects[2].m_position.z = 1.0f;
			s_StaticOpaqueObjects[2].m_position.x = 1.5f;

			if (!s_StaticOpaqueObjects[3].m_Mesh.LoadMeshFromFile("data/Pipe.lua"))
				return false;
			s_StaticOpaqueObjects[3].m_Material.LoadMaterial("data/OpaqueShaderMaterial_red.lua");

			s_StaticOpaqueObjects[3].m_position.z = 1.0f;
			s_StaticOpaqueObjects[3].m_position.x = 3.5f;
		}

		s_StaticTransparentObjects = new eae6320::Graphics::Renderable[2];

		if (!s_StaticTransparentObjects[0].m_Mesh.LoadMeshFromFile("data/Pipe.lua"))
			return false;
		s_StaticTransparentObjects[0].m_Material.LoadMaterial("data/TransparentShaderMaterial_semi.lua");
		s_StaticTransparentObjects[0].m_position.x = -3.0f;
		s_StaticTransparentObjects[0].m_position.z = 1.0f;

		if (!s_StaticTransparentObjects[1].m_Mesh.LoadMeshFromFile("data/Pipe.lua"))
			return false;
		s_StaticTransparentObjects[1].m_Material.LoadMaterial("data/TransparentShaderMaterial_almostsemi.lua");
		s_StaticTransparentObjects[1].m_position.x = -1.0f;
		s_StaticTransparentObjects[1].m_position.z = 1.0f;

		return true;
	}

	bool CreateCamera()
	{
		s_Camera = new eae6320::Graphics::Camera(0, 0, 10);
		return s_Camera != NULL;
	}
}
