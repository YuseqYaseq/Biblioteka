//Sprite.h

#pragma once

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <xnamath.h>
#include <vector>


#include "Animation.h"

namespace PORTECZKI
{
	class AO
	{
	public:
		AO();
		~AO();

		void LoadFromFile(char* pszFilename);

		fRect GetPosition(int iAnimation, int iFrame);
		int GetNumOfAnimations(){return m_iNumberOfAnimations;}
		int GetNumOfFrames(int iAnimation){return m_pAnimations[iAnimation].GetNumOfFrames();}

		void SetActive();

		void Render(XMMATRIX* pModelMatrix, int iAnimation);
		float GetTimer(int iAnimation){return m_pAnimations[iAnimation].GetTimer();}
		int GetRestart(int iAnimation){return m_pAnimations[iAnimation].GetRestart();}

		int GetCellHeight(){return m_kTemplate.GetCellHeight();}
		int GetCellWidth(){return m_kTemplate.GetCellWidth();}

		int GetTexWidth(){return m_kTexture.GetDesc().Width;}
		int GetTexHeight(){return m_kTexture.GetDesc().Height;}
		

	private:
		Texture		m_kTexture;
		AnimationTemplate	m_kTemplate;
		int			m_iNumberOfAnimations;
		std::vector<Animation>	m_pAnimations;
		std::vector<std::vector<CellID> >m_pPositions;
	};
};