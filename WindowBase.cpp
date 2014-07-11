//WindowBase.cpp

#include "WindowBase.h"

using namespace PORTECZKI;

WindowBase::WindowBase()
{

}

WindowBase::~WindowBase()
{

}

void WindowBase::Initialize(int iX, int iY, HINSTANCE hInstance)
{
	WNDCLASSEX	WndClass	= {0};
	WndClass.cbSize			= sizeof(WNDCLASSEX);
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= WndProc;
	WndClass.hInstance		= hInstance;
	WndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW - 2);
	WndClass.lpszMenuName	= NULL;
	WndClass.lpszClassName	= "DX11WindowBaseClass";

	m_hInstance = hInstance;

	if(!RegisterClassEx(&WndClass)) return;

	RECT rc = {0, 0, iX, iY};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd	= CreateWindowA("DX11WindowBaseClass", "Default title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rc.right - rc.left), (rc.bottom - rc.top), NULL, NULL, hInstance, NULL);

	if(!m_hwnd) return;

	ShowWindow(m_hwnd, 1);

	LoadContent();
}

int WindowBase::Run()
{
	MSG msg = {0};
	Timer timer;
	timer.Update();
	m_bExit	= false;

	while(!(msg.message == WM_QUIT || m_bExit == true))
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Update();
			Update(static_cast<float>(timer.GetDelta()));
			Render(static_cast<float>(timer.GetDelta()));
		}
	}
	return static_cast<int>(msg.wParam);
}

void WindowBase::SetTitle(char* pszTitle)
{
	SetWindowText(m_hwnd, pszTitle);
}

void WindowBase::SetPosition(int iX, int iY)
{
	SetWindowPos(m_hwnd, 0, iX, iY, 0, 0, SWP_NOSIZE);
}

void WindowBase::SetSize(int iX, int iY)
{
	SetWindowPos(m_hwnd, 0, 0, 0, iX, iY, SWP_NOMOVE);
}






LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT PaintStruct;
	HDC hDC;

	switch(message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &PaintStruct);
		EndPaint(hwnd, &PaintStruct);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}