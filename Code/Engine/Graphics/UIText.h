#pragma once

#include "UIElement.h"

namespace eae6320
{
	namespace Graphics
	{
		namespace UI
		{
			class UIText : public UIElement
			{
			public:
				void Create(void *i_pObjectProperty, const char* i_pLabel);
				void Draw(LPRECT i_drawRect);
				void Update();

			private:
				char *m_TextProperty;
				char *m_Label;
			};
		}
	}
}
