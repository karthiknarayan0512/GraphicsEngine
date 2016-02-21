#include "UIText.h"

#include "DebugMenu.h"

namespace eae6320
{
	namespace Graphics
	{
		namespace UI
		{
			void UIText::Create(void *i_pObjectProperty)
			{
				m_TextProperty = reinterpret_cast<char *>(i_pObjectProperty);
			}

			void UIText::Draw(LPRECT i_drawRect)
			{
				DebugMenu::GetFont()->DrawTextA(NULL, m_TextProperty, -1, i_drawRect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 0));
			}
		}
	}
}