#include "DebugMenu.h"

#include "Context.h"

#include <cassert>
#include <vector>

namespace eae6320
{
	namespace Graphics
	{
		namespace DebugMenu
		{
			LPD3DXFONT g_pDebugMenuFont;

			std::vector<eae6320::Graphics::UI::UIElement *> s_UIElements;

			void DrawUIElements()
			{
				for (size_t i = 0; i < s_UIElements.size(); i++)
					s_UIElements[i]->Draw(NULL, 0, 255, 0);
			}

			void AddUIElement(eae6320::Graphics::UI::UIElement * i_pUIElement)
			{
				s_UIElements.push_back(i_pUIElement);
			}
			
			void CreateDebugMenuFont()
			{
				HRESULT result = D3DXCreateFont(Context::getDirect3DDevice(),     //D3D Device

					22,               //Font height

					0,                //Font width

					FW_NORMAL,        //Font Weight

					1,                //MipLevels

					false,            //Italic

					DEFAULT_CHARSET,  //CharSet

					OUT_DEFAULT_PRECIS, //OutputPrecision

					ANTIALIASED_QUALITY, //Quality

					DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily

					"Arial",          //pFacename,

					&g_pDebugMenuFont);         //ppFont

				assert(SUCCEEDED(result));
			}

			LPD3DXFONT GetFont()
			{
				return g_pDebugMenuFont;
			}
		}
	}
}