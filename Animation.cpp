//Animation.cpp

#include "Animation.h"

using namespace PORTECZKI;

Animation::Animation()
{
	ZeroMemory(this, sizeof(*this));
}

Animation::~Animation()
{
	RELEASE_COM(m_mvpCB);
}

void Animation::Initialize(int iNumberOfFrames, bool bRestart, float fTimer)
{
	m_bRestart	= bRestart;
	m_iFrames	= iNumberOfFrames;
	m_fTime		= fTimer;

	D3D11_BUFFER_DESC	ConstDesc;
	ZeroMemory(&ConstDesc, sizeof(ConstDesc));
	ConstDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstDesc.ByteWidth	= sizeof(XMMATRIX);
	ConstDesc.Usage		= D3D11_USAGE_DEFAULT;
	if(FAILED(Device::Get()->GetDevice()->CreateBuffer(&ConstDesc, 0, &m_mvpCB)))
	{
		DXTRACE_MSG("Failed to create constant buffer!");
		return;
	}
}

void Animation::Render(XMMATRIX* pModelMatrix)
{
	//XMMATRIX temp = g_vpMatrix;
	XMMATRIX mvp = XMMatrixMultiply(*pModelMatrix, Device::Get()->GetVPMatrix());
	mvp = XMMatrixTranspose(mvp);
	Device::Get()->GetContext()->UpdateSubresource(m_mvpCB, 0, 0, &mvp, 0, 0);
	Device::Get()->GetContext()->VSSetConstantBuffers(0, 1, &m_mvpCB);
	Device::Get()->GetContext()->Draw(6, 0);
}