//Keyboard.h interfejs klawiury

#pragma once

#pragma comment(lib, "dinput8.lib")

#include "InputManager.h"

namespace PORTECZKI
{
	class Keyboard
	{
	public:
		Keyboard();
		~Keyboard();

		void Initialize(HINSTANCE hInstance, HWND hwnd);
		void Update();

		bool KeyDown(int iKey);
		bool KeyUp(int iKey);
		bool KeyPressed(int iKey);
		bool KeyReleased(int iKey);

	private:
		char		m_KeyboardKeys[256];
		char		m_PrevKeyboardKeys[256];

		LPDIRECTINPUTDEVICE8	m_KeyboardDevice;
	};
};