//Keyboard.cpp

#include "Keyboard.h"

using namespace PORTECZKI;

Keyboard::Keyboard() : m_KeyboardDevice(0)
{
	ZeroMemory(m_KeyboardKeys, sizeof(char) * 256);
	ZeroMemory(m_PrevKeyboardKeys, sizeof(char) * 256);
}

Keyboard::~Keyboard()
{
	if(m_KeyboardDevice)
	{
		m_KeyboardDevice->Unacquire();
		m_KeyboardDevice->Release();
	}
	m_KeyboardDevice = 0;
}

void Keyboard::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	if(FAILED(InputManager::Get()->GetDevice()->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, 0)))
	{
		DXTRACE_MSG("Failed to create keyboard device!");
		return;
	}

	if(FAILED(m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		DXTRACE_MSG("Failed to set keyboard data format!");
		return;
	}

	if(FAILED(m_KeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		DXTRACE_MSG("Failed to set keyboard cooperative level!");
		return;
	}

	if(FAILED(m_KeyboardDevice->Acquire()))
	{
		DXTRACE_MSG("Failed to acquire keyboard device!");
		return;
	}
}

void Keyboard::Update()
{
	memcpy(m_PrevKeyboardKeys, m_KeyboardKeys, sizeof(m_KeyboardKeys));
	if(FAILED(m_KeyboardDevice->GetDeviceState(sizeof(m_KeyboardKeys), (LPVOID)&m_KeyboardKeys)))
	{
		m_KeyboardDevice->Acquire();
		m_KeyboardDevice->GetDeviceState(sizeof(m_KeyboardKeys), (LPVOID)&m_KeyboardKeys);
	}
}

bool Keyboard::KeyDown(int iKey)
{
	return static_cast<bool>(m_KeyboardKeys[iKey] & 0x80);
}

bool Keyboard::KeyUp(int iKey)
{
	return static_cast<bool>(!(m_KeyboardKeys[iKey] & 0x80));
}

bool Keyboard::KeyPressed(int iKey)
{
	return static_cast<bool>((m_KeyboardKeys[iKey] & 0x80) && !(m_PrevKeyboardKeys[iKey] & 0x80));
}

bool Keyboard::KeyReleased(int iKey)
{
	return static_cast<bool>(!(m_KeyboardKeys[iKey] & 0x80) && (m_PrevKeyboardKeys[iKey] & 0x80));
}