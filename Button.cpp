//Button.cpp

#include "Button.h"

using namespace PORTECZKI;

Button::Button() : m_eState(MouseOut), m_pMouse(0)
{

}

Button::~Button()
{

}

void Button::Initialize(AO* pAO, Mouse* pMouse, Font* pFont, const char* szText, XMFLOAT2& rPos, XMFLOAT2& rScale)
{
	//m_kRect		= rRect;
	m_eState	= MouseOut;
	m_pMouse	= pMouse;

	m_kSprite.Initialize(pAO);
	m_kSprite.SetAnimation(0);

	m_kSprite.SetPosition(rPos);
	m_kSprite.SetScale(rScale);

	m_pFont = pFont;
	m_szText = szText;
}

void Button::Render()
{
	m_kSprite.Render();
	XMFLOAT2 Temp = m_pFont->GetScale();
	///////
	float CharW	= m_pFont->GetCharWidth() * Device::Get()->GetWidth();
	float Length	= strlen(m_szText) * CharW;// * m_kSprite.GetScale().x;
	float Width	= m_kSprite.GetWidth();// * m_kSprite.GetScale().x;

	float XScale;
	if(strlen(m_szText) < 6) XScale = (Width) / (Length + (CharW / 2.0f));
	else XScale = (Width) / (Length + (CharW));


	
	//////
	m_pFont->SetScale(XMFLOAT2((XScale), (XScale)));

	float temp = m_kSprite.GetPosition().x - (m_kSprite.GetWidth() / 2.2f) + (CharW * XScale / 2.0f);

	m_pFont->Render(m_szText, XMFLOAT2(temp, (m_kSprite.GetPosition().y)));
	m_pFont->SetScale(Temp);
}

void Button::Update()
{
	float X = (float)m_pMouse->GetXPos();
	float Y = (float)m_pMouse->GetYPos();

	
	
	while(true)
	{
		//Sprawdzamy, czy mysz znajduje siê wewn¹trz przycisku
		//if(	(X > (m_kSprite.GetPosition().x) - (m_kSprite.GetWidth() / 2)) && (X < (m_kSprite.GetPosition().x + (m_kSprite.GetWidth() / 2))) &&
		//	(Y > (m_kSprite.GetPosition().y) - (m_kSprite.GetHeight() / 2)) && (Y < (m_kSprite.GetPosition().y + (m_kSprite.GetHeight() / 2))))
		if(m_kSprite.CheckCollision(X, Y))
		{
			if(m_pMouse->KeyPressed(0))				 m_eState = MouseKeyPressed;
			if(m_pMouse->KeyDown(0) && (m_eState == MouseKeyPressed || m_eState == MouseKeyDown))
			{
				m_eState = MouseKeyDown;
				break;
			}
			if(m_pMouse->KeyReleased(0) && m_eState == MouseKeyDown)
			{
				m_eState = MouseReleased;
				break;
			}
			m_eState = MouseIn; break;
		}
		else
		{
			m_eState = MouseOut;
			break;
		}

	}
	m_kSprite.SetAnimation(static_cast<int>(m_eState));
}

ButtonState Button::GetState()
{
	return m_eState;
}

void Button::SetState(ButtonState eState)
{
	m_eState = eState;
}