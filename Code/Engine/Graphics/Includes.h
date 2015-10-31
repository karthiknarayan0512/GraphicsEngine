#ifndef GRAPHICS_INCLUDES_H
#define GRAPHICS_INCLUDES_H

#ifdef _DEBUG
#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
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
	}

}

#endif
