//Font.cpp

#include "Font.h"

using namespace PORTECZKI;

Font::Font()
{
	m_pBuffer = 0;
	m_cBuffer = 0;
}

Font::~Font()
{
	RELEASE_COM(m_pBuffer);
	RELEASE_COM(m_cBuffer);
}

void Font::Initialize(const char* pszFileName)
{
	static const float NumOfLetters = 95.0f;
	m_kTexture.Initialize(pszFileName);
	D3D11_TEXTURE2D_DESC	TexDesc = m_kTexture.GetDesc();
	DXGI_SWAP_CHAIN_DESC SCDesc;
	Device::Get()->GetSwapChain()->GetDesc(&SCDesc);
	m_CharHeight	= ((float)TexDesc.Height) / ((float)SCDesc.BufferDesc.Height);
	m_CharWidth		= ((float)TexDesc.Width / NumOfLetters)  / ((float)SCDesc.BufferDesc.Width);
	m_TexelWidth	= (1.0f / NumOfLetters);

	VertexPos TempVPos[6 * 95];

	float HalfHeight	= (float)TexDesc.Height / 2.0f;
	float HalfWidth		= (float)TexDesc.Width / (NumOfLetters * 2);

	for(int i = 0; i < NumOfLetters; ++i)
	{
		TempVPos[0 + (i * 6)].pos = XMFLOAT3( HalfWidth,  HalfHeight, 1.0f);TempVPos[0 + (i * 6)].tex0 = XMFLOAT2(m_TexelWidth * (1 + i), 0.0f);
		TempVPos[1 + (i * 6)].pos = XMFLOAT3( HalfWidth, -HalfHeight, 1.0f);TempVPos[1 + (i * 6)].tex0 = XMFLOAT2(m_TexelWidth * (1 + i), 1.0f);
		TempVPos[2 + (i * 6)].pos = XMFLOAT3(-HalfWidth, -HalfHeight, 1.0f);TempVPos[2 + (i * 6)].tex0 = XMFLOAT2(m_TexelWidth * (0 + i), 1.0f);
		TempVPos[3 + (i * 6)].pos = XMFLOAT3(-HalfWidth, -HalfHeight, 1.0f);TempVPos[3 + (i * 6)].tex0 = XMFLOAT2(m_TexelWidth * (0 + i), 1.0f);
		TempVPos[4 + (i * 6)].pos = XMFLOAT3(-HalfWidth,  HalfHeight, 1.0f);TempVPos[4 + (i * 6)].tex0 = XMFLOAT2(m_TexelWidth * (0 + i), 0.0f);
		TempVPos[5 + (i * 6)].pos = XMFLOAT3( HalfWidth,  HalfHeight, 1.0f);TempVPos[5 + (i * 6)].tex0 = XMFLOAT2(m_TexelWidth * (1 + i), 0.0f);
	}

	D3D11_SUBRESOURCE_DATA	ResourceData;
	ZeroMemory(&ResourceData, sizeof(ResourceData));
	ResourceData.pSysMem	= &TempVPos;

	D3D11_BUFFER_DESC	BDesc;
	ZeroMemory(&BDesc, sizeof(BDesc));
	BDesc.Usage	= D3D11_USAGE_DYNAMIC;
	BDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	BDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	BDesc.ByteWidth			= sizeof(VertexPos) * 6 * static_cast<const int>(NumOfLetters);

	if(FAILED(Device::Get()->GetDevice()->CreateBuffer(&BDesc, &ResourceData, &m_pBuffer)))
	{
		DXTRACE_MSG("Failed to create font buffer!");
		return;
	}

	D3D11_BUFFER_DESC	ConstDesc;
	ZeroMemory(&ConstDesc, sizeof(ConstDesc));
	ConstDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstDesc.ByteWidth	= sizeof(XMMATRIX);
	ConstDesc.Usage		= D3D11_USAGE_DEFAULT;
	if(FAILED(Device::Get()->GetDevice()->CreateBuffer(&ConstDesc, 0, &m_cBuffer)))
	{
		DXTRACE_MSG("Failed to create constant buffer!");
		return;
	}

	m_Rotation = 0;
	m_Scale = XMFLOAT2(1.0f, 1.0f);
	m_Position = XMFLOAT2(0.0f, 0.0f);
}

void Font::Render(const char* pszText, XMFLOAT2& rPos)
{
	int Length = strlen(pszText);

	const int Index1 = (' ');
	const int IndexLast = ('~');
	/*
	D3D11_MAPPED_SUBRESOURCE	MapResource;
	if(FAILED(Device::Get()->GetContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MapResource)))
	{
		DXTRACE_MSG("Failed to map font buffer!");
		return;
	}
	VertexPos*	VertexPtr = (VertexPos*)MapResource.pData;
	*/
	if(rPos.x == -1.0f && rPos.y == -1.0f) rPos = m_Position;
	for(int i = 0; i < Length; i++)
	{
		
		
		int TexLookup = 0;
		int Letter = pszText[i];
		if(Letter < Index1 || Letter > IndexLast)
		{
			TexLookup = (IndexLast - Index1) + 1;
		}
		else
		{
			TexLookup = Letter - Index1;
		}

		float TStart	= (m_TexelWidth * (float)TexLookup);
		float TEnd		= TStart + m_TexelWidth;

		

		XMMATRIX Rotation = XMMatrixRotationZ(m_Rotation);
		XMMATRIX Position = XMMatrixTranslation(rPos.x + m_Scale.x * (((float)i * 23.0f)), rPos.y, 0.0f);
		XMMATRIX Scale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.0f);

		XMMATRIX world = XMMatrixTranspose(Scale * Rotation * Position * Device::Get()->GetVPMatrix());



		//Device::Get()->GetContext()->Unmap(m_pBuffer, 0);
		unsigned int Stride	= sizeof(VertexPos);
		unsigned int Offset	= TexLookup * sizeof(VertexPos) * 6;



		m_kTexture.SetActive();
		Device::Get()->GetContext()->UpdateSubresource(m_cBuffer, 0, 0, &world, 0, 0);
		Device::Get()->GetContext()->VSSetConstantBuffers(0, 1, &m_cBuffer);
		Device::Get()->GetContext()->IASetVertexBuffers(0, 1, &m_pBuffer, &Stride, &Offset);
		Device::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Device::Get()->GetContext()->Draw(6, 0);
	}
}

