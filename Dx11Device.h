//Dx11Device.h

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DxErr.lib")

#include <Windows.h>
#include <d3d11.h>
#include <DxErr.h>
#include <D3Dcompiler.h>
#include <D3DX11async.h>
#include <xnamath.h>

#include "Defines.h"
#include "WindowBase.h"


namespace PORTECZKI
{
	struct VertexPos
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex0;
	};

	class Device
	{
	public:

		void Initialize(HINSTANCE hInstance, HWND hwnd, bool bWindowed);
		bool CompileShader(char* FilePath, char* Entry, char* ShaderModel, ID3DBlob** Buffer);

		void BeginScene(float fColor[4]);
		void EndScene(void);

		void InitVertexShader(char* FilePath, char* Entry, char* ShaderModel);
		void InitPixelShader(char* FilePath, char* Entry, char* ShaderModel);

		//void Resize(HINSTANCE hInstance, HWND hwnd, bool bWindowed, int width, int height);
		void Release(){m_Device.~Device();}

		ID3D11Device*			GetDevice()	{ return m_d3dDevice; }
		ID3D11DeviceContext*	GetContext() { return m_d3dContext; }
		IDXGISwapChain*			GetSwapChain() { return m_SwapChain; }
		ID3D11RenderTargetView*	GetBackBuffer() { return m_BackBufferTarget; }

		int GetWidth(){return m_iWidth;}
		int GetHeight(){return m_iHeight;}
		XMMATRIX GetVPMatrix();

		static Device*	Get() {return &m_Device;}

		XMFLOAT2	m_kCameraPos;

	private:

		//********
		Device();
		~Device();
		//********
		int m_iWidth;
		int m_iHeight;

		//XMFLOAT4X4				m_vpMatrix;

		ID3D11Device*			m_d3dDevice;
		ID3D11DeviceContext*	m_d3dContext;
		IDXGISwapChain*			m_SwapChain;
		ID3D11RenderTargetView*	m_BackBufferTarget;

		D3D_DRIVER_TYPE			m_DriverType;
		D3D_FEATURE_LEVEL		m_FeatureLevel;

		ID3D11InputLayout*	m_InputLayout;
		ID3D11VertexShader*	m_VertexShader;
		ID3D11PixelShader*	m_PixelShader;

		ID3D11SamplerState*			m_ColorMapSampler;
		ID3D11BlendState*			m_AlphaBlendState;

		static Device			m_Device;
	};
};
//#define g_vpMatrix (XMMatrixMultiply(XMMatrixIdentity(), XMMatrixOrthographicOffCenterLH(0.0f, 640.0f, 0.0f, 480.0f, 0.1f, 100.0f)))