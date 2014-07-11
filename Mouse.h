//Mouse.h

#pragma once

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <DxErr.h>

#include "InputManager.h"
#include "Dx11Device.h"

namespace PORTECZKI
{
	class Mouse
	{
	public:
		Mouse();
		~Mouse();

		void Initialize(HINSTANCE hInstance, HWND hwnd);
		void Update();

		bool KeyDown(int iKey);
		bool KeyUp(int iKey);
		bool KeyPressed(int iKey);
		bool KeyReleased(int iKey);

		int GetXAxis();
		int GetYAxis();
		int GetZAxis();

		int GetXPos();
		int GetYPos();

	private:
		int						m_MousePosX;
		int						m_MousePosY;
		int						m_MouseWheel;
		DIMOUSESTATE			m_MouseState;
		DIMOUSESTATE			m_PrevMouseState;

		LPDIRECTINPUTDEVICE8	m_MouseDevice;

		HWND					m_hwnd;
	};
};