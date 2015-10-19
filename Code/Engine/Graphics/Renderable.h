#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Mesh.h"
#include "Effect.h"
#include "..\Math\cVector.h"

namespace eae6320
{
	class Renderable
	{
	public:
		Mesh m_Mesh;
		Effect m_Effect;
		Math::cVector m_positionOffset;

		void Render();
	};
}

#endif
