//InputManager.cpp

#include "InputManager.h"

using namespace PORTECZKI;

InputManager InputManager::m_kInputManager;

InputManager::InputManager() : m_Device(0)
{

}

InputManager::~InputManager()
{
	RELEASE_COM(m_Device);
}

void InputManager::Initialize(HINSTANCE hInstance)
{
	if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Device, 0)))
	{
		DXTRACE_MSG("Failed to create direct input object!");
		return;
	}
}

LPDIRECTINPUT8	InputManager::GetDevice()
{
	return m_Device;
}

InputManager*	InputManager::Get()
{
	return &m_kInputManager;
}