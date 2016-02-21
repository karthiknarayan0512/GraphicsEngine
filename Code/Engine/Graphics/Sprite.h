#pragma once

#include "Material.h"
#include "../Math/cQuaternion.h"
#include "../Math/cVector.h"
#include "Camera.h"

#include <d3d9.h>

namespace eae6320
{
	namespace Graphics
	{
		class Sprite
		{
		public:
			void CreateTexture(const char *i_MaterialPath, float left, float right, float top, float bottom, float u, float v);
			void SetSprite(Camera &i_Camera);
			void UpdateUVs(float i_URate, float i_vRate);
		private:
			Material m_textureMaterial;
			Math::cVector m_position;
			Math::cQuaternion m_Orientation;
			Effect m_Effect;

			IDirect3DVertexBuffer9 *m_VertexBuffer;

			float m_u, m_v;
			float left, right, top, bottom;
		};
	}
}