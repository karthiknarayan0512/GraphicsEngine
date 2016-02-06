#include "..\Texture.h"
#include "..\Context.h"

#include <D3dx9tex.h>
#include <cassert>

namespace eae6320
{
	namespace Graphics
	{
		void Texture::LoadTexture(const char *i_texturePath)
		{
			IDirect3DDevice9* direct3dDevice = Context::getDirect3DDevice();

			const unsigned int useDimensionsFromFile = D3DX_DEFAULT_NONPOW2;
			const unsigned int useMipMapsFromFile = D3DX_FROM_FILE;
			const DWORD staticTexture = 0;
			const D3DFORMAT useFormatFromFile = D3DFMT_FROM_FILE;
			const D3DPOOL letD3dManageMemory = D3DPOOL_MANAGED;
			const DWORD useDefaultFiltering = D3DX_DEFAULT;
			const D3DCOLOR noColorKey = 0;
			D3DXIMAGE_INFO* noSourceInfo = NULL;
			PALETTEENTRY* noColorPalette = NULL;
			const HRESULT result = D3DXCreateTextureFromFileEx(direct3dDevice, i_texturePath, useDimensionsFromFile, useDimensionsFromFile, useMipMapsFromFile,
				staticTexture, useFormatFromFile, letD3dManageMemory, useDefaultFiltering, useDefaultFiltering, noColorKey, noSourceInfo, noColorPalette,
				&m_Texture);
			assert(SUCCEEDED(result));
		}

		void Texture::SetTexture(size_t i_textureUnit)
		{
			IDirect3DDevice9* direct3dDevice = Context::getDirect3DDevice();

			HRESULT result = direct3dDevice->SetTexture(m_SamplerID, m_Texture);
			assert(SUCCEEDED(result));
		}
	}
}