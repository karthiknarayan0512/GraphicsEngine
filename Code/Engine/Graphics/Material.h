#ifndef MATERIAL_H
#define MATERIAL_H

#include "Effect.h"
#include "Includes.h"
#include "Camera.h"
#include "Texture.h"
#include "../Math/cMatrix_transformation.h"

namespace eae6320
{
	namespace Graphics
	{
		class Material
		{
		public:
			struct sParameterToSet
			{
				tUniformHandle uniformHandle;
				ShaderTypes::eShaderType shaderType;
				float values[4];
				uint8_t valueCountToSet;
			};

			void LoadMaterial(const char *i_materialPath);
			void SetMaterial(Math::cMatrix_transformation i_localToWorldTransform, Camera &i_Camera, bool bSetTransforms = true);
		private:
			Effect m_Effect;
			sParameterToSet *m_Uniforms;
			uint8_t m_UniformCount;
			uint8_t m_TextureCount;
			Texture* m_Textures;
		};
	}
}

#endif