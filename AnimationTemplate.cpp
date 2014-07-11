//AnimationTemplate.cpp

#include "AnimationTemplate.h"

using namespace PORTECZKI;

AnimationTemplate::AnimationTemplate() : m_iCellWidth(0), m_iCellHeight(0), m_iTexWidth(0),
										m_iTexHeight(0)
{
	
}

AnimationTemplate::~AnimationTemplate()
{

}

void AnimationTemplate::Initialize(Texture* pTexture, int iCellWidth, int iCellHeight)
{
	D3D11_TEXTURE2D_DESC TexDesc = pTexture->GetDesc();
	m_iCellWidth		= iCellWidth;
	m_iCellHeight		= iCellHeight;
	m_iTexWidth			= TexDesc.Width;
	m_iTexHeight		= TexDesc.Height;
}

fRect	AnimationTemplate::GetFrame(int iCellX, int iCellY)
{
	fRect kRect;
	kRect.top		= static_cast<float>((((float)iCellY * ((float)m_iCellHeight))) / (float)m_iTexHeight);
	kRect.bottom	= static_cast<float>(kRect.top + ((float)m_iCellHeight / (float)m_iTexHeight));
	kRect.left		= static_cast<float>((((float)iCellX * ((float)m_iCellWidth))) / (float)m_iTexWidth);
	kRect.right		= static_cast<float>((float)kRect.left + ((float)m_iCellWidth / (float)m_iTexWidth));
	return kRect;
}

fRect AnimationTemplate::GetFrame(CellID* ID)
{
	return GetFrame(ID->X, ID->Y);
}