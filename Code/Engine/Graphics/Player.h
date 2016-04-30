#include "Renderable.h"
#include "Camera.h"

#include <vector>

namespace eae6320
{
	namespace Graphics
	{
		class Player
		{
		private:
			Renderable *m_Renderables;
		public:
			Player()
			{
				m_Renderables = new Renderable[11];
			}
			void AddPlayerRenderable(const char* i_PlayerMeshFile, const char * i_PlayerMaterialFile);
			void RenderPlayer(Camera &i_Camera);
		};
	}
}
