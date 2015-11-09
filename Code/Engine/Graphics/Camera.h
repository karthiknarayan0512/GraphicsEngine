#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"

namespace eae6320
{
	namespace Graphics
	{
		class Camera
		{
		private:
			Math::cVector m_Position;
			Math::cQuaternion m_Orientation;
		public:
			void UpdateCameraPosition(float &i_positionOffset_x, float &i_positionOffset_y);
			Math::cVector getCameraPosition() { return m_Position; }
			Math::cQuaternion getCameraOrientation() { return m_Orientation; }
		};
	}
}

#endif