#pragma once

#include "../Graphics/Mesh.h"
#include "../Math/cVector.h"

namespace eae6320
{
	namespace Physics
	{
		void Initialize(const char * i_CollisionMesh);
		bool CheckCollisions(const Math::cVector &i_StartPoint, const Math::cVector &i_EndPoint, Math::cVector &i_IntersectionPoint);
	}
}