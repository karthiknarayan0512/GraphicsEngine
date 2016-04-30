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

			Math::cVector m_LeftVector;
		public:
			Math::cVector m_ForwardVector;
			Camera::Camera(float i_x, float i_y, float i_z);
			void UpdateCameraPosition(Math::cVector &i_Position, bool bThirdPerson = false);
			Math::cVector getCameraPosition() { return m_Position; }
			Math::cQuaternion getCameraOrientation() { return m_Orientation; }
			void UpdateCameraOrientation(float &i_angleInRadians, Math::cVector& i_axisNormalized);
		};
	}
}

#endif