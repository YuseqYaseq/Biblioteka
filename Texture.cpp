//Texture.cpp

#include "Texture.h"

using namespace PORTECZKI;

Texture::Texture() : m_Texture(0)
{

}

Texture::~Texture()
{
	RELEASE_COM(m_Texture);
}

void Texture::Initialize(const char* lpszFilename)
{
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(Device::Get()->GetDevice(), lpszFilename, 0, 0, &m_Texture, 0)))
	{
		DXTRACE_MSG("Failed to create a texture ");
		return;
	}
}

ID3D11ShaderResourceView*	Texture::GetResource()
{
	return m_Texture;
}

D3D11_TEXTURE2D_DESC	Texture::GetDesc()
{
	ID3D11Resource*	TexRes;
	m_Texture->GetResource(&TexRes);

	D3D11_TEXTURE2D_DESC	ColorTexDesc;
	((ID3D11Texture2D*)TexRes)->GetDesc(&ColorTexDesc);
	TexRes->Release();
	return ColorTexDesc;
}

void Texture::SetActive()
{
	Device::Get()->GetContext()->PSSetShaderResources(0, 1, &m_Texture);


}