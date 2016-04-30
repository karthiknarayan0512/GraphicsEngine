#include "Player.h"

namespace eae6320
{
	namespace Graphics
	{	
		void Player::AddPlayerRenderable(const char* i_PlayerMeshFile, const char * i_PlayerMaterialFile)
		{
			static int currentIndex = 0;
			m_Renderables[currentIndex++] = Renderable(i_PlayerMeshFile, i_PlayerMaterialFile);			
		}

		void Player::RenderPlayer(Camera &i_Camera)
		{
			for (size_t i = 0; i < 11; i++)
				m_Renderables[i].Render(i_Camera);
		}
	}
}