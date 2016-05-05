#include "UIText.h"

#include "DebugMenu.h"

namespace eae6320
{
	namespace Graphics
	{
		namespace UI
		{
			void UIText::Create(void *i_pObjectProperty, const char* i_pLabel)
			{
				m_TextProperty = reinterpret_cast<char *>(i_pObjectProperty);
			}

			void UIText::Draw(LPRECT i_drawRect, size_t r, size_t g, size_t b)
			{
				DebugMenu::GetFont()->DrawTextA(NULL, m_TextProperty, -1, i_drawRect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(r, g, b));
			}

			void UIText::Update()
			{

			}
		}
	}
}