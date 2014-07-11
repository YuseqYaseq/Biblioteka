//AnimationTemplate.h

#pragma once

#include "Texture.h"

namespace PORTECZKI
{
	class CellID
	{
	public:
		int X;
		int Y;
	};

	class AnimationTemplate
	{
	public:
		AnimationTemplate();
		~AnimationTemplate();

		void Initialize(Texture* pTexture, int iCellWidth, int iCellHeight);
		fRect GetFrame(int iCellX, int iCellY);
		fRect GetFrame(CellID* ID);
		int GetCellWidth(){return m_iCellWidth;}
		int GetCellHeight(){return m_iCellHeight;}

	private:
		int		m_iCellWidth;
		int		m_iCellHeight;
		int		m_iTexWidth;
		int		m_iTexHeight;
	};
};