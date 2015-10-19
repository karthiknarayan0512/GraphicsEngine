#ifndef EFFECT_H
#define EFFECT_H

#ifdef EAE6320_PLATFORM_GL
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include <gl/GLU.h>
#endif

#ifdef EAE6320_PLATFORM_D3D
#include <d3d9.h>
#include <d3dx9shader.h>
#endif

namespace eae6320
{
	class Effect
	{
	public:
		bool CreateEffect(const char *i_vertexShaderFile, const char *i_fragmentShaderfile);
		void SetEffect();
		void SetPositionOffset(float* i_positionOffset);

		~Effect();

	private:
		bool LoadVertexShader(const char *i_vertexShaderFile);

		bool LoadFragmentShader(const char *i_fragmentShaderfile);

#ifdef EAE6320_PLATFORM_D3D
		IDirect3DVertexShader9* m_vertexShader;

		IDirect3DPixelShader9* m_fragmentShader;

		D3DXHANDLE m_positionOffset;

		ID3DXConstantTable* m_vertexShaderConstantTable;

#elif defined EAE6320_PLATFORM_GL
		GLuint m_location;

		GLuint m_programID;

		bool LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage);

		bool CreateProgram(const char *i_vertexShaderFile, const char *i_fragmentShaderfile);
#endif
	};
}

#endif
