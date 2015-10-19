#include "Renderable.h"

namespace eae6320
{
	void Renderable::Render()
	{
		m_Effect.SetEffect();

		float positionOffset[2] = { m_positionOffset.x, m_positionOffset.y };
		m_Effect.SetPositionOffset(positionOffset);

		m_Mesh.DrawMesh();
	}
}