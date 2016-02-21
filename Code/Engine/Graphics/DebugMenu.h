#pragma once

#include "UIElement.h"

#include <d3dx9core.h>

namespace eae6320
{
	namespace Graphics
	{
		namespace DebugMenu
		{
			void CreateDebugMenuFont();
			LPD3DXFONT GetFont();

			void AddUIElement(eae6320::Graphics::UI::UIElement * i_pUIElement);
			void DrawUIElements();
		}
	}
}
