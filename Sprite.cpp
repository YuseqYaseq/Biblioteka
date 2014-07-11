//Sprite.cpp

#include "Sprite.h"

using namespace PORTECZKI;

Sprite::Sprite()
{
	ZeroMemory(this, sizeof(*this));
}

Sprite::~Sprite()
{
	RELEASE_COM(m_pBuffer);
}

XMMATRIX Sprite::GetWorldMatrix(void)
{
	XMMATRIX	Scale		= XMMatrixScaling(m_kScale.x, m_kScale.y, 1.0f);
	XMMATRIX	Translation	= XMMatrixTranslation(m_kPosition.x, m_kPosition.y, 1.0f);
	XMMATRIX	Rotation	= XMMatrixRotationZ(m_fRotation);

	return Scale * Rotation * Translation;
}


void Sprite::SetAnimation(int iAnimation)
{
	if(iAnimation > m_pAO->GetNumOfAnimations()) return;
	m_iCurrentAnimation = iAnimation;
	m_iCurrentFrame		= 0;
	m_fTimer			= 0.0f;
}

void Sprite::Render()
{
	fRect temp = m_pAO->GetPosition(m_iCurrentAnimation, m_iCurrentFrame);
	
	D3D11_MAPPED_SUBRESOURCE	ResData;
	ZeroMemory(&ResData, sizeof(ResData));
	if(FAILED(Device::Get()->GetContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &ResData)))
	{
		DXTRACE_MSG("Failed to map resource!");
		return;
	}
	VertexPos*	VPos = (VertexPos*) ResData.pData;

	VPos[0].tex0	= XMFLOAT2(temp.right, temp.top);
	VPos[1].tex0	= XMFLOAT2(temp.right, temp.bottom);
	VPos[2].tex0	= XMFLOAT2(temp.left, temp.bottom);
	VPos[3].tex0	= XMFLOAT2(temp.left, temp.bottom);
	VPos[4].tex0	= XMFLOAT2(temp.left, temp.top);
	VPos[5].tex0	= XMFLOAT2(temp.right, temp.top);

	Device::Get()->GetContext()->Unmap(m_pBuffer, 0);

	unsigned int Stride	= sizeof(VertexPos);
	unsigned int Offset	= 0;

	m_pAO->SetActive();
	Device::Get()->GetContext()->IASetVertexBuffers(0, 1, &m_pBuffer, &Stride, &Offset);
	Device::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pAO->Render(&GetWorldMatrix(), m_iCurrentAnimation);
}

void Sprite::Update(float dt)
{
	m_fTimer += dt;
	if(m_fTimer >= m_pAO->GetTimer(m_iCurrentAnimation))
	{
		if((m_iCurrentFrame + 1) < m_pAO->GetNumOfFrames(m_iCurrentAnimation)) ++m_iCurrentFrame;
		else if(m_pAO->GetRestart(m_iCurrentAnimation)) m_iCurrentFrame = 0;

		m_fTimer -= m_pAO->GetTimer(m_iCurrentAnimation);
	}
}

void Sprite::Initialize(AO* pAO)
{
	
	m_iCurrentAnimation = 0;
	m_iCurrentFrame		= 0;
	m_fRotation			= 0.0f;
	m_kPosition			= XMFLOAT2(0.0f, 0.0f);
	m_kScale			= XMFLOAT2(1.0f, 1.0f);
	m_pAO				= pAO;
	m_fTimer			= 0.0f;

	int X = m_pAO->GetCellWidth();
	int Y = m_pAO->GetCellHeight();

	float HalfWidth = X / 2.0f;
	float HalfHeight = Y / 2.0f;

	VertexPos TempVPos[6];

	TempVPos[0].pos	= XMFLOAT3( HalfWidth,  HalfHeight, 1.0f);
	TempVPos[1].pos	= XMFLOAT3( HalfWidth, -HalfHeight, 1.0f);
	TempVPos[2].pos	= XMFLOAT3(-HalfWidth, -HalfHeight, 1.0f);
	TempVPos[3].pos	= XMFLOAT3(-HalfWidth, -HalfHeight, 1.0f);
	TempVPos[4].pos	= XMFLOAT3(-HalfWidth,  HalfHeight, 1.0f);
	TempVPos[5].pos	= XMFLOAT3( HalfWidth,  HalfHeight, 1.0f);

	D3D11_BUFFER_DESC	VertexDesc;
	ZeroMemory(&VertexDesc, sizeof(VertexDesc));
	VertexDesc.Usage	= D3D11_USAGE_DYNAMIC;
	VertexDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	VertexDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	VertexDesc.ByteWidth		= sizeof(VertexPos) * 6;

	D3D11_SUBRESOURCE_DATA	ResourceData;
	ZeroMemory(&ResourceData, sizeof(ResourceData));
	ResourceData.pSysMem	= &TempVPos;
	Device::Get()->GetDevice()->CreateBuffer(&VertexDesc, &ResourceData, &m_pBuffer);
}

bool Sprite::CheckCollision(float fWidth, float fHeight)
{
	float X = GetWidth() / 2.0f;
	float Y = GetHeight() / 2.0f;

	if(fWidth < m_kPosition.x - X) return false;
	if(fWidth > m_kPosition.x + X) return false;
	if(fHeight < m_kPosition.y - Y) return false;
	if(fHeight > m_kPosition.y + Y) return false;

	return true;
}

bool Sprite::CheckCollision(Sprite*	pSprite)
{
	float X = pSprite->GetWidth() / 2.0f;
	float Y = pSprite->GetHeight() / 2.0f;

	if(CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y - Y))return true;// Lewy,  dolny róg 2. obr.
	if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y - Y))return true;// Prawy, dolny róg 2. obr.
	if(CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y + Y))return true;// Lewy,  górny róg 2. obr.
	if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y + Y))return true;// Prawy, górny róg 2. obr.

	X = GetWidth() / 2.0f;
	Y = GetHeight() / 2.0f;

	if(pSprite->CheckCollision(GetPosition().x - X, GetPosition().y - Y))return true;// Lewy,  dolny róg tego obr.
	if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y - Y))return true;// Prawy, dolny róg tego obr.
	if(pSprite->CheckCollision(GetPosition().x - X, GetPosition().y + Y))return true;// Lewy,  górny róg tego obr.
	if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y + Y))return true;// Prawy, górny róg tego obr.

	return false;
}

bool Sprite::CheckCollisionDown(Sprite* pSprite)
{
	float X = pSprite->GetWidth() / 2.0f;
	float Y = pSprite->GetHeight() / 2.0f;

	if(CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y + Y))// Lewy,  górny róg 2. obr.
		if(!CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y - Y))return true;// Lewy,  dolny róg 2. obr.
		else if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y + Y))return true;// Prawy, górny róg 2. obr.
	if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y + Y))// Prawy, górny róg 2. obr.
		if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y - Y))return true;// Prawy, dolny róg 2. obr.

	X = GetWidth() / 2.0f;
	Y = GetHeight() / 2.0f;

	if(pSprite->CheckCollision(GetPosition().x - X, GetPosition().y - Y))// Lewy,  dolny róg tego obr.
		if(!pSprite->CheckCollision(GetPosition().x - X, GetPosition().y + Y))return true;// Lewy,  górny róg tego obr.
		else if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y - Y))return true;// Prawy, dolny róg tego obr.
	if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y - Y))// Prawy, dolny róg tego obr.
		if(!pSprite->CheckCollision(GetPosition().x + X, GetPosition().y + Y))return true;// Prawy, górny róg tego obr.

	return false;
}

bool Sprite::CheckCollisionUp(Sprite* pSprite)
{
	float X = pSprite->GetWidth() / 2.0f;
	float Y = pSprite->GetHeight() / 2.0f;

	if(CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y - Y))// Lewy,  dolny róg 2. obr.
		if(!CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y + Y))return true;// Lewy,  górny róg 2. obr.
		else if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y - Y))return true;// Prawy, dolny róg 2. obr.
	if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y - Y))// Prawy, dolny róg 2. obr.
		if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y + Y))return true;// Prawy, górny róg 2. obr.

	X = GetWidth() / 2.0f;
	Y = GetHeight() / 2.0f;

	if(pSprite->CheckCollision(GetPosition().x - X, GetPosition().y + Y))// Lewy,  górny róg tego obr.
		if(!pSprite->CheckCollision(GetPosition().x - X, GetPosition().y - Y))return true;// Lewy,  dolny róg tego obr.
		else if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y + Y))return true;// Prawy, górny róg tego obr.
	if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y + Y))// Prawy, górny róg tego obr.
		if(!pSprite->CheckCollision(GetPosition().x + X, GetPosition().y - Y))return true;// Prawy, dolny róg tego obr.

	return false;
}

bool Sprite::CheckCollisionLeft(Sprite* pSprite)
{
	float X = pSprite->GetWidth() / 2.0f;
	float Y = pSprite->GetHeight() / 2.0f;

	if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y + Y))return true;// Prawy, górny róg 2. obr.
	if(CheckCollision(pSprite->GetPosition().x + X, pSprite->GetPosition().y - Y))return true;// Prawy, dolny róg 2. obr.

	X = GetWidth() / 2.0f;
	Y = GetHeight() / 2.0f;

	if(pSprite->CheckCollision(GetPosition().x - X, GetPosition().y + Y))return true;// Lewy,  górny róg tego obr.
	if(pSprite->CheckCollision(GetPosition().x - X, GetPosition().y - Y))return true;// Lewy,  dolny róg tego obr.

	return false;
}

bool Sprite::CheckCollisionRight(Sprite* pSprite)
{
	float X = pSprite->GetWidth() / 2.0f;
	float Y = pSprite->GetHeight() / 2.0f;

	if(CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y - Y))return true;// Lewy,  dolny róg 2. obr.
	if(CheckCollision(pSprite->GetPosition().x - X, pSprite->GetPosition().y + Y))return true;// Lewy,  górny róg 2. obr.

	X = GetWidth() / 2.0f;
	Y = GetHeight() / 2.0f;

	if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y - Y))return true;// Prawy, dolny róg tego obr.
	if(pSprite->CheckCollision(GetPosition().x + X, GetPosition().y + Y))return true;// Prawy, górny róg tego obr.

	return false;
}

void Sprite::SetSize(float width, float height)
{
	m_kScale.x = width  / (float)m_pAO->GetCellWidth();
	m_kScale.y = height / (float)m_pAO->GetCellHeight();
}

