//Texture.h

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DX11.lib")

#include <d3d11.h>

#include "Dx11Device.h"

namespace PORTECZKI
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		void Initialize(const char* lpszFileName);
		ID3D11ShaderResourceView*	GetResource();
		D3D11_TEXTURE2D_DESC	GetDesc();
		void SetActive();

	private:
		ID3D11ShaderResourceView*	m_Texture;
	};
};