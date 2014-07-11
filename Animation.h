//Animation.h

#pragma once


#include <xnamath.h>
#include "AnimationTemplate.h"

namespace PORTECZKI
{
	class Animation
	{
	public:
		Animation();
		~Animation();

		void Initialize(int iNumberOfFrames, bool bRestart, float fTimer);
		void Render(XMMATRIX* pModelMatrix);
		int  GetNumOfFrames() {return m_iFrames;}

		float GetTimer(){return m_fTime;}
		bool GetRestart(){return m_bRestart;}

	private:
		float				m_fTime;
		int					m_iFrames;
		bool				m_bRestart;
		ID3D11Buffer*		m_mvpCB;
	};
};