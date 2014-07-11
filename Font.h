//Font.h

#pragma once


#include "Dx11Device.h"
#include "Texture.h"

#define NUM_OF_LETTERS 95

namespace PORTECZKI
{
	class Font
	{
	public:
		Font();
		~Font();

		void Initialize(const char* lpszFileName);
		void Render(const char* pszText, XMFLOAT2& rPos = XMFLOAT2(-1.0f, -1.0f));

		void SetPosition(XMFLOAT2& rPosition){m_Position = rPosition;}
		void SetRotation(float fRotation){m_Rotation = fRotation;}
		void SetScale(XMFLOAT2& rScale){m_Scale = rScale;}

		XMFLOAT2 GetPosition(){return m_Position;}
		XMFLOAT2 GetScale(){return m_Scale;}
		float GetRotation(){return m_Rotation;}

		float	GetCharWidth(){return m_CharWidth;}
		float	GetCharHeight(){return m_CharHeight;}
		float	GetTexelWidth(){return m_TexelWidth;}



	private:
		Texture		m_kTexture;
		float		m_CharWidth;
		float		m_CharHeight;
		float		m_TexelWidth;
		ID3D11Buffer*	m_pBuffer;
		ID3D11Buffer*	m_cBuffer;
		float		m_Rotation;
		XMFLOAT2	m_Position;
		XMFLOAT2	m_Scale;
	};
};