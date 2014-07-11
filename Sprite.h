//Sprite.h

#pragma once

#include "AO.h"

namespace PORTECZKI
{
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void Initialize(AO* pAO);

		void SetPosition(XMFLOAT2& rPosition){m_kPosition = rPosition;}
		void SetScale(XMFLOAT2& rScale){m_kScale = rScale;}
		void SetSize(float width, float height);
		void SetRotation(float fRotation){m_fRotation = fRotation;}

		XMFLOAT2& GetPosition(){return m_kPosition;}
		XMFLOAT2& GetScale(){return m_kScale;}
		float&	 GetRotation(){return m_fRotation;}
		float	 GetWidth(){return (m_pAO->GetCellWidth() * m_kScale.x);}
		float	 GetHeight(){return (m_pAO->GetCellHeight() * m_kScale.y);}
		AO*		 GetAO(){return m_pAO;}

		XMMATRIX GetWorldMatrix(void);

		void Render();
		void Update(float dt);

		void SetAnimation(int iAnimation);

		bool CheckCollision(Sprite*	pSprite);
		bool CheckCollision(float fWidth, float fHeight);

		bool CheckCollisionUp(Sprite* pSprite);
		bool CheckCollisionDown(Sprite* pSprite);
		bool CheckCollisionLeft(Sprite* pSprite);
		bool CheckCollisionRight(Sprite* pSprite);

	private:
		float		m_fRotation;
		XMFLOAT2	m_kPosition;
		XMFLOAT2	m_kScale;
		int			m_iCurrentAnimation;
		AO*			m_pAO;
		ID3D11Buffer*	m_pBuffer;
		int			m_iCurrentFrame;
		float		m_fTimer;
	};
};