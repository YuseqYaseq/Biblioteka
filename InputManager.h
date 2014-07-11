//InputManager.h klasa zarz�dzaj�ca urz�dzeniami wej�cia

#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <DxErr.h>
#include "Defines.h"



namespace PORTECZKI
{
	class InputManager
	{
	public:
		

		void Initialize(HINSTANCE hInstance);

		LPDIRECTINPUT8	GetDevice();
		static InputManager*	Get();

	private:
		LPDIRECTINPUT8	m_Device;
		//////////////
		InputManager();
		~InputManager();
		/////////////
		static InputManager m_kInputManager;
	};
};