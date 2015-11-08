#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Mesh.h"
#include "Effect.h"
#include "..\Math\cVector.h"
#include "..\Math\cQuaternion.h"
#include "Camera.h"

namespace eae6320
{
	namespace Graphics
	{
		class Renderable
		{
		public:
			Mesh m_Mesh;
			Effect m_Effect;
			Math::cVector m_position;
			Math::cQuaternion m_orientation;

			void Render(Camera &i_Camera);
		};
	}
}

#endif
