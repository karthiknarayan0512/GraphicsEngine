#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/cVector.h"

namespace eae6320
{
	namespace Graphics
	{
		class Camera
		{
		private:
			Math::cVector m_Position;
		public:
			void UpdateCameraPosition(float &i_positionOffset_x, float &i_positionOffset_y);
			Math::cVector getCameraPosition() { return m_Position; }
		};
	}
}

#endif