#ifndef CONTEXT_H
#define CONTEXT_H

#include <d3d9.h>

namespace eae6320
{
	namespace Graphics
	{
		namespace Context
		{
			bool Initialize(const HWND i_renderingWindow);
			void ClearBuffers();
			void BeginRender();
			void EndRender();
			bool Shutdown();
			void SwapBuffers();

			float getAspectRatio();
#ifdef EAE6320_PLATFORM_D3D
			IDirect3DDevice9* getDirect3DDevice();
#endif
		};
	}
}

#endif
