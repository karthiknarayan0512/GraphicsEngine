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
		void SendFlagLocation();
		bool IsInitialized();
		bool DidIGetTagged();
		void TagBitch();
		void SendScore(int i_Score);
	}
}
