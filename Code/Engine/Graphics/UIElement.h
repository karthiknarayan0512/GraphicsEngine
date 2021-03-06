#pragma once

#include "../Windows/Includes.h"

namespace eae6320
{
	namespace Graphics
	{
		namespace UI
		{
			class UIElement
			{
			public:
				virtual void Create(void *i_pObjectProperty, const char* i_pLabel) = 0;
				virtual void Update() = 0;
				virtual void Draw(LPRECT i_drawRect, size_t r, size_t g, size_t b) = 0;

				char *m_Label;
			};
		}
	}
}
