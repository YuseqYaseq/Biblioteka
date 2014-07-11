//AO.cpp

#include <fstream>
#include <string>
#include "AO.h"

using namespace PORTECZKI;

AO::AO()
{
	ZeroMemory(this, sizeof(*this));
}

AO::~AO()
{
	
}

void AO::LoadFromFile(char* pszFilename)
{
	std::fstream	kFile(pszFilename);
	if(!kFile.is_open())
	{
		DXTRACE_MSG("Failed to open file!");
		return;
	}
	std::string	TexFileName;
	kFile >> TexFileName;
	m_kTexture.Initialize(TexFileName.c_str());

	int iCellWidth, iCellHeight;
	kFile >> iCellWidth >> iCellHeight;
	m_kTemplate.Initialize(&m_kTexture, iCellWidth, iCellHeight);


	kFile >> m_iNumberOfAnimations;
	m_pAnimations.resize(m_iNumberOfAnimations);
	m_pPositions.resize(m_iNumberOfAnimations);

	for(int i = 0; i < m_iNumberOfAnimations; ++i)
	{
		int iFrames;
		bool bRestart;
		float fTimer;
		kFile >> iFrames >> bRestart >> fTimer;
		m_pAnimations[i].Initialize(iFrames, bRestart, fTimer);

		m_pPositions[i].resize(iFrames);
		for(int j = 0; j < iFrames; ++j)
		{
			kFile >> m_pPositions[i][j].Y;
			kFile >> m_pPositions[i][j].X;
		}
	}

}

fRect AO::GetPosition(int iAnimation, int iFrame)
{
	return m_kTemplate.GetFrame(&m_pPositions[iAnimation][iFrame]);
}

void AO::Render(XMMATRIX* pModelMatrix, int iAnimation)
{
	m_pAnimations[iAnimation].Render(pModelMatrix);
}

void AO::SetActive()
{
	m_kTexture.SetActive();
}