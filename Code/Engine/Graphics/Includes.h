#ifndef GRAPHICS_INCLUDES_H
#define GRAPHICS_INCLUDES_H

#ifdef _DEBUG
#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

#include <Windows.h>

#ifdef EAE6320_PLATFORM_GL
#include <gl\GL.h>
#endif

namespace eae6320
{
	namespace Graphics
	{
		namespace ShaderTypes
		{
			enum eShaderType
			{
				Unknown = 0,
				Fragment = 1,
				Vertex = 2
			};
		}

#if defined( EAE6320_PLATFORM_D3D )
		typedef const char* tUniformHandle;
#elif defined( EAE6320_PLATFORM_GL )
		typedef GLint tUniformHandle;
#endif

#ifdef EAE6320_PLATFORM_D3D
		typedef DWORD SamplerID;
#elif defined(EAE6320_PLATFORM_GL)
		typedef GLint SamplerID;
#endif
	}

}

#endif
