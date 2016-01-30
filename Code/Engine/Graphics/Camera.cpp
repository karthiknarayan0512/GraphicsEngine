#include "Camera.h"
#include "../Math/cMatrix_transformation.h"

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
			Math::cVector movementVector(i_positionOffset_x, 0.0f, i_positionOffset_z);
			Math::cMatrix_transformation transformationMatrix(m_Orientation, movementVector);
			movementVector = movementVector * transformationMatrix;

			m_Position += movementVector;
		}

		void Camera::UpdateCameraOrientation(float &i_angleInRadians, Math::cVector& i_axisNormalized)
		{
			Math::cQuaternion rotation(i_angleInRadians, i_axisNormalized);

			m_Orientation = m_Orientation * rotation;
		}
	}
}