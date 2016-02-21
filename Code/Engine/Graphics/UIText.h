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
				void Create(void *i_pObjectProperty);
				void Draw(LPRECT i_drawRect);

			private:
				char *m_TextProperty;
			};
		}
	}
}
