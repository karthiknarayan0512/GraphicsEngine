#ifndef TEXTURE_H
#define TEXTURE_H

#include "Includes.h"

#include <d3d9.h>

namespace eae6320
{
	namespace Graphics
	{
		class Texture
		{
		public:
			void SetSamplerID(SamplerID i_samplerID)
			{
				m_SamplerID = i_samplerID;
			}
			void LoadTexture(const char *i_texturePath);
			void SetTexture(size_t i_textureUnit);
		private:

#ifdef EAE6320_PLATFORM_D3D
			IDirect3DTexture9* m_Texture;
			SamplerID m_SamplerID;
#elif defined EAE6320_PLATFORM_GL
			GLuint m_Texture;
			GLint m_SamplerID;
#endif


		};
	}
}

#endif
