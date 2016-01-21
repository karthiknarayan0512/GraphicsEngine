#include "Camera.h"

namespace eae6320
{
	namespace Graphics
	{
		Camera::Camera(float i_x, float i_y, float i_z) :
			m_Position(i_x, i_y, i_z)
		{
		}
		void Camera::UpdateCameraPosition(float &i_positionOffset_x, float &i_positionOffset_z)
		{
			m_Position.x += i_positionOffset_x;
			m_Position.z += i_positionOffset_z;
		}
	}
}