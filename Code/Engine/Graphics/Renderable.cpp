#include "Renderable.h"
#include "../Math/cMatrix_transformation.h"

namespace eae6320
{
	namespace Graphics
	{
		void Renderable::Render(Camera &i_Camera)
		{
			Math::cMatrix_transformation i_localToWorldTransform(m_orientation, m_position);
			m_Material.SetMaterial(i_localToWorldTransform, i_Camera);

			m_Mesh.DrawMesh();
		}
	}
}