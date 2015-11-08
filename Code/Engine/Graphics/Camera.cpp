#include "Camera.h"

namespace eae6320
{
	namespace Graphics
	{
		void Camera::UpdateCameraPosition(float &i_positionOffset_x, float &i_positionOffset_y)
		{
			m_Position.x += i_positionOffset_x;
			m_Position.y += i_positionOffset_y;
			m_Position.z = 10.0;
		}
	}
}