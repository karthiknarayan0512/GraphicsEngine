#include "../Graphics/Graphics.h"

namespace eae6320
{
	namespace Networking
	{
		void CreateClient();
		void CreateServer();
		void Update();
		void Destroy();
		Graphics::Renderable* GetConnectedPlayers();
		bool IsServer();
		void SendPlayerPosition();
		bool IsInitialized();
	}
}
