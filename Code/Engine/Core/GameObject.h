#pragma once

#include "../Math/cVector.h"
#include "../Graphics/Renderable.h"

namespace eae6320
{
	namespace Core
	{
		class GameObject
		{
		public:
			Math::cVector getVelocity() const { return m_Velocity; }
			Math::cVector getPosition() const { return m_Position; }

			void setVelocity(const Math::cVector &i_Velocity) { m_Velocity = i_Velocity; }
			void setPosition(const Math::cVector &i_Position) { m_Position = i_Position; }

		private:
			Math::cVector m_Velocity;
			Math::cVector m_Position;
		};
	}
}
