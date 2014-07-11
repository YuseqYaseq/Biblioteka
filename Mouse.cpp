//Mouse.cpp

#include "Mouse.h"

using namespace PORTECZKI;

Mouse::Mouse()
{
	ZeroMemory(this, sizeof(Mouse));
}

Mouse::~Mouse()
{
	if(m_MouseDevice)
	{
		m_MouseDevice->Unacquire();
		m_MouseDevice->Release();
	}
	m_MouseDevice	= 0;
}

void Mouse::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	m_hwnd = hwnd;
	if(FAILED(InputManager::Get()->GetDevice()->CreateDevice(GUID_SysMouse, &m_MouseDevice, 0)))
	{
		DXTRACE_MSG("Failed to create mouse device!");
		return;
	}

	if(FAILED(m_MouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		DXTRACE_MSG("Failed to set mouse data format!");
		return;
	}

	if(FAILED(m_MouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		DXTRACE_MSG("Failed to set mouse cooperative level!");
		return;
	}

	if(FAILED(m_MouseDevice->Acquire()))
	{
		DXTRACE_MSG("Failed to acquire mouse!");
		return;
	}
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	m_MousePosX = pt.x;
	m_MousePosY = pt.y;
	
}

void Mouse::Update()
{
	memcpy(&m_PrevMouseState, &m_MouseState, sizeof(m_MouseState));

	if(FAILED(m_MouseDevice->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState)))
	{
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);
	}
	//m_MousePosX	+= m_MouseState.lX;
	//m_MousePosY	+= m_MouseState.lY;
	m_MouseWheel += m_MouseState.lZ;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hwnd, &pt);
	pt.y = (Device::Get()->GetHeight() - pt.y);


	m_MouseState.lX = m_MousePosX - pt.x;
	m_MouseState.lY = m_MousePosY - pt.y;

	m_MousePosX = pt.x;
	m_MousePosY = pt.y;
}

bool Mouse::KeyDown(int iKey)
{
	return static_cast<bool>(m_MouseState.rgbButtons[iKey] & 0x80);
}

bool Mouse::KeyUp(int iKey)
{
	return static_cast<bool>(!(m_MouseState.rgbButtons[iKey] & 0x80));
}

bool Mouse::KeyPressed(int iKey)
{
	return static_cast<bool>((m_MouseState.rgbButtons[iKey] & 0x80) && !(m_PrevMouseState.rgbButtons[iKey] & 0x80));
}

bool Mouse::KeyReleased(int iKey)
{
	return static_cast<bool>(!(m_MouseState.rgbButtons[iKey] & 0x80) && (m_PrevMouseState.rgbButtons[iKey] & 0x80));
}

int Mouse::GetXAxis()
{
	return m_MouseState.lX;
}

int Mouse::GetYAxis()
{
	return m_MouseState.lY;
}

int Mouse::GetZAxis()
{
	return m_MouseState.lZ;
}

int Mouse::GetXPos()
{
	return m_MousePosX;
}

int Mouse::GetYPos()
{
	return m_MousePosY;
}
