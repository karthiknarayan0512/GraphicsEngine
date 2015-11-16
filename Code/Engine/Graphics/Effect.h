#ifndef EFFECT_H
#define EFFECT_H

#include "../Math/cMatrix_transformation.h"
#include "Camera.h"
#ifdef EAE6320_PLATFORM_GL
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include <gl/GLU.h>
#endif

#ifdef EAE6320_PLATFORM_D3D
#include <d3d9.h>
#include <d3dx9shader.h>
#endif

#include <stdint.h>

#define ALPHA_TRANSPARENCY 1
#define DEPTH_TESTING 2
#define DEPTH_WRITING 3

namespace eae6320
{
	namespace Graphics
	{
		class Effect
		{
		public:
			bool CreateEffect(const char *i_shaderBinaryFile);
			void SetEffect();
			void SetTransforms(Math::cMatrix_transformation i_localToWorldTransform, Camera &i_Camera);

			~Effect();

		private:
			bool LoadVertexShader(const char *i_vertexShaderFile);

			bool LoadFragmentShader(const char *i_fragmentShaderfile);

			uint8_t m_renderStates;

#ifdef EAE6320_PLATFORM_D3D
			IDirect3DVertexShader9* m_vertexShader;

			IDirect3DPixelShader9* m_fragmentShader;

			D3DXHANDLE m_LocalToWorldTransform;
			D3DXHANDLE m_WorldToViewTransform;
			D3DXHANDLE m_ViewToScreenTransform;

			ID3DXConstantTable* m_vertexShaderConstantTable;

#elif defined EAE6320_PLATFORM_GL
			GLuint m_LocalToWorldTransform;
			GLuint m_WorldToViewTransform;
			GLuint m_ViewToScreenTransform;

			GLuint m_programID;

			bool LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage);

			bool CreateProgram(const char *i_vertexShaderFile, const char *i_fragmentShaderfile);
#endif
		};
	}
}

#endif
