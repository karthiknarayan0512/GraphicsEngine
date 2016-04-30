#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Mesh.h"
#include "..\Math\cVector.h"
#include "..\Math\cQuaternion.h"
#include "Material.h"
#include "Camera.h"

namespace eae6320
{
	namespace Graphics
	{
		class Renderable
		{
		public:
			Renderable()
			{

			}

			Renderable(const char *i_MeshFile, const char *i_MaterialFile)
			{
				m_Mesh.LoadMeshFromFile(i_MeshFile);
				m_Material.LoadMaterial(i_MaterialFile);
			}
			Mesh m_Mesh;
			Material m_Material;
			Math::cVector m_position;
			Math::cQuaternion m_orientation;

			void Render(Camera &i_Camera);
		};
	}
}

#endif
