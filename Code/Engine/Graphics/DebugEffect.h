#pragma once

#include "../Math/cMatrix_transformation.h"
#include "Camera.h"
#include "Includes.h"

#include <d3d9.h>
#include <d3dx9shader.h>

#include <stdint.h>

namespace eae6320
{
	namespace Graphics
	{
		class DebugEffect
		{
		public:
			bool CreateEffect(const char *i_shaderBinaryFile);
			void SetEffect();
			void SetTransforms(Camera &i_Camera);

		private:
			bool LoadVertexShader(const char *i_vertexShaderFile);

			bool LoadFragmentShader(const char *i_fragmentShaderfile);

			tUniformHandle m_WorldToViewTransform;
			tUniformHandle m_ViewToScreenTransform;

			IDirect3DVertexShader9* m_vertexShader;

			IDirect3DPixelShader9* m_fragmentShader;

			ID3DXConstantTable* m_vertexShaderConstantTable;
		};
	}
}


