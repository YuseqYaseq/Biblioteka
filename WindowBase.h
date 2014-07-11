//WindowBase.h klasa okna

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DxErr.lib")

#include <Windows.h>
#include <d3d11.h>

#include "Timer.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace PORTECZKI
{
	
	
	class WindowBase
	{
	public:
		WindowBase();
		virtual ~WindowBase();

		void Initialize(int iX, int iY, HINSTANCE hInstance);
		int Run(void);

		void SetSize(int iX, int iY);
		void SetPosition(int iX, int iY);
		void SetTitle(char* pszTitle);

		virtual void Update(float dt) = 0;
		virtual void Render(float dt) = 0;

		virtual void LoadContent() = 0;
		virtual void UnloadContent() = 0;

		HWND GetHWND() {return m_hwnd;}

	protected:
		HINSTANCE	m_hInstance;
		HWND		m_hwnd;
		bool		m_bExit;
		

	};
};
