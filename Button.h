//Button.h

#pragma once

#include "Dx11Device.h"
#include "Mouse.h"
#include "Sprite.h"
#include "Font.h"

namespace PORTECZKI
{
	enum ButtonState
	{
		MouseOut,
		MouseIn,
		MouseKeyDown,
		MouseReleased,
		MouseKeyPressed,

	};
	
	class Button
	{
	public:
		Button();
		~Button();

		void Initialize(AO* pAO, Mouse* pMouse, Font* pFont, const char* szText, XMFLOAT2& rPos, XMFLOAT2& rScale = XMFLOAT2(1.0f, 1.0f));

		void Update();
		void Render();

		ButtonState GetState();
		void SetState(ButtonState eState);

		Sprite*	GetSprite(){return &m_kSprite;}

	private:
		//fRect	m_kRect;
		ButtonState	m_eState;
		Sprite	m_kSprite;
		Mouse*	m_pMouse;
		Font*	m_pFont;
		const char*	m_szText;
	};
};