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
				virtual void Create(void *i_pObjectProperty) = 0;
				virtual void Update() = 0;
				virtual void Draw(LPRECT i_drawRect) = 0;
			};
		}
	}
}
