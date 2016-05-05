#include "Camera.h"
#include "../Math/cMatrix_transformation.h"

#include "../Time/Time.h"
#include "../Physics/Physics.h"

namespace eae6320
{
	namespace Graphics
	{
		Camera::Camera(float i_x, float i_y, float i_z) :
			m_Position(i_x, i_y, i_z),
			m_ForwardVector(0.0f, 0.0f, -1.0f),
			m_LeftVector(-1.0f, 0.0f, 0.0f)
		{
		}
		
		void Camera::UpdateCameraPosition(Math::cVector &i_Position, bool bThirdPerson)
		{
			if (bThirdPerson)
			{
				eae6320::Math::cMatrix_transformation i_localToWorldTransformBB8 = eae6320::Math::cMatrix_transformation(m_Orientation, i_Position);

				eae6320::Math::cVector camOffset = eae6320::Math::cVector(0, 10, 100);
				eae6320::Math::cVector val = camOffset * i_localToWorldTransformBB8;
				eae6320::Math::cVector newPosition = m_Position + (val - m_Position) * eae6320::Time::GetSecondsElapsedThisFrame() * 5;

				eae6320::Math::cVector intersectionPoint;
				//if (Physics::CheckCollisions(newPosition, i_Position, intersectionPoint))
				//{
				//	newPosition = intersectionPoint;
				//}
				m_Position = newPosition;
			}
			else
			{
				Math::cVector Zero;
				Math::cMatrix_transformation nonTranslationMatrix(m_Orientation, Zero);

				Math::cVector movementVector = i_Position * nonTranslationMatrix;
				m_Position += movementVector;
			}
		}

		void Camera::UpdateCameraOrientation(float &i_angleInRadians, Math::cVector& i_axisNormalized)
		{
			Math::cQuaternion rotation(i_angleInRadians, i_axisNormalized);
			m_Orientation = m_Orientation * rotation;

			if (i_angleInRadians != 0.0f)
			{
				Math::cVector Zero;
				Math::cMatrix_transformation nonTranslationMatrix(rotation, Zero);

				m_ForwardVector = m_ForwardVector * nonTranslationMatrix;
			}
		}
	}
}