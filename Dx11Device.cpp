//Dx11Device.cpp

#include "Dx11Device.h"

using namespace PORTECZKI;
Device Device::m_Device;
//static XMFLOAT4X4	g_vpMatrix;


Device::Device() : m_d3dDevice(0), m_d3dContext(0), m_SwapChain(0), m_BackBufferTarget(0), m_InputLayout(0),
				m_VertexShader(0), m_PixelShader(0), m_ColorMapSampler(0), m_AlphaBlendState(0)
{

}

Device::~Device()
{
	RELEASE_COM(m_d3dDevice);
	RELEASE_COM(m_d3dContext);
	RELEASE_COM(m_SwapChain);
	RELEASE_COM(m_BackBufferTarget);
	RELEASE_COM(m_InputLayout);
	RELEASE_COM(m_VertexShader);
	RELEASE_COM(m_PixelShader);
	RELEASE_COM(m_ColorMapSampler);
	RELEASE_COM(m_AlphaBlendState);
}

void Device::Initialize(HINSTANCE hInstance, HWND hwnd, bool bWindowed)
{
	RECT dimension;
	GetClientRect(hwnd, &dimension);

	unsigned int width	= dimension.right - dimension.left;
	unsigned int height	= dimension.bottom - dimension.top;

	m_iWidth = width;
	m_iHeight = height;


	D3D_DRIVER_TYPE DriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	D3D_FEATURE_LEVEL FeatureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	unsigned int TotalDriverTypes = ARRAYSIZE(DriverTypes);
	unsigned int TotalFeatureLevels = ARRAYSIZE(FeatureLevels);

	DXGI_SWAP_CHAIN_DESC	SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.BufferCount			= 1;
	SwapChainDesc.BufferDesc.Width		= width;
	SwapChainDesc.BufferDesc.Height		= height;
	SwapChainDesc.BufferDesc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	SwapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	SwapChainDesc.OutputWindow			= hwnd;
	SwapChainDesc.SampleDesc.Count		= 1;
	SwapChainDesc.SampleDesc.Quality	= 0;
	SwapChainDesc.Windowed				= bWindowed;

	unsigned int CreationFlags	= 0;

#if	defined(_DEBUG) || defined(DEBUG)
	//CreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hResult;
	unsigned int Driver;

	for(Driver = 0; Driver < TotalDriverTypes; ++Driver)
	{
		hResult = D3D11CreateDeviceAndSwapChain(0, DriverTypes[Driver], 0, CreationFlags, FeatureLevels,
								TotalFeatureLevels, D3D11_SDK_VERSION, &SwapChainDesc, &m_SwapChain, 
								&m_d3dDevice, &m_FeatureLevel, &m_d3dContext);

		if(SUCCEEDED(hResult))
		{
			m_DriverType	= DriverTypes[Driver];
			break;
		}
	}
	if(FAILED(hResult) || (m_d3dDevice == 0))
	{
		DXTRACE_MSG("Failed to create the Direct3D device!");
		return;
	}

	ID3D11Texture2D*	BackBufferTexture;
	hResult	= m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);
	if(FAILED(hResult))
	{
		DXTRACE_MSG("Failed to get the swap chain back buffer!");
		return;
	}

	hResult	= m_d3dDevice->CreateRenderTargetView(BackBufferTexture, 0, &m_BackBufferTarget);
	RELEASE_COM(BackBufferTexture);
	if(FAILED(hResult))
	{
		DXTRACE_MSG("Failed to create the render target view!");
		return;
	}

	m_d3dContext->OMSetRenderTargets(1, &m_BackBufferTarget, 0);

	D3D11_VIEWPORT	Viewport;
	Viewport.Width	= static_cast<float>(width);
	Viewport.Height	= static_cast<float>(height);
	Viewport.MinDepth	= 0.0f;
	Viewport.MaxDepth	= 1.0f;
	Viewport.TopLeftX	= 0.0f;
	Viewport.TopLeftY	= 0.0f;

	m_d3dContext->RSSetViewports(1, &Viewport);
	
	D3D11_SAMPLER_DESC	ColorMapDesc;
	ZeroMemory(&ColorMapDesc, sizeof(ColorMapDesc));
	ColorMapDesc.AddressU		= D3D11_TEXTURE_ADDRESS_MIRROR;
	ColorMapDesc.AddressV		= D3D11_TEXTURE_ADDRESS_MIRROR;
	ColorMapDesc.AddressW		= D3D11_TEXTURE_ADDRESS_MIRROR;
	ColorMapDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	ColorMapDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	ColorMapDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	if(FAILED(m_d3dDevice->CreateSamplerState(&ColorMapDesc, &m_ColorMapSampler)))
	{
		DXTRACE_MSG("Failed to create color map sampler state!");
		return;
	}

	D3D11_BLEND_DESC	BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.RenderTarget[0].BlendEnable	= TRUE;
	BlendDesc.RenderTarget[0].BlendOp		= D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend		= D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOpAlpha	= D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	float BlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	
	if(FAILED(m_d3dDevice->CreateBlendState(&BlendDesc, &m_AlphaBlendState)))
	{
		DXTRACE_MSG("Failed to create blend state!");
		return;
	}
	m_d3dContext->OMSetBlendState(m_AlphaBlendState, BlendFactor, 0xFFFFFFFF);

	//float Dodx = 32.0f;
	//float Dody = 32.0f;

	//XMStoreFloat4x4(&m_vpMatrix, XMMatrixOrthographicOffCenterLH(0.0f + Dodx, (float)width + Dodx, 0.0f + Dody, (float)height + Dody, 0.1f, 100.0f));
}

bool Device::CompileShader(char* FilePath, char* Entry, char* ShaderModel, ID3DBlob** Buffer)
{
	DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	ShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob*	ErrorBuffer	= 0;
	HRESULT result = D3DX11CompileFromFile(FilePath, 0, 0, Entry, ShaderModel, ShaderFlags, 0, 0, Buffer, &ErrorBuffer, 0);
	if(FAILED(result))
	{
		if(ErrorBuffer != 0)
		{
			OutputDebugStringA((char*)(ErrorBuffer->GetBufferPointer()));
			ErrorBuffer->Release();
		}
		return false;
	}
	if(ErrorBuffer)	ErrorBuffer->Release();
	return true;
}

void Device::InitVertexShader(char* FilePath, char* Entry, char* ShaderModel)
{
	ID3DBlob*	vsBuffer	= 0;
	if(!CompileShader(FilePath, Entry, ShaderModel, &vsBuffer))
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return;
	}

	if(FAILED(m_d3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &m_VertexShader)))
	{
		if(vsBuffer)vsBuffer->Release();
		return;
	}
	
	D3D11_INPUT_ELEMENT_DESC SolidColorLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	unsigned int TotalLayoutElements = ARRAYSIZE(SolidColorLayout);

	if(FAILED(m_d3dDevice->CreateInputLayout(SolidColorLayout, TotalLayoutElements, vsBuffer->GetBufferPointer(), 
		vsBuffer->GetBufferSize(), &m_InputLayout)))
	{
		vsBuffer->Release();
		DXTRACE_MSG("Error creating input layout!");
		return;
	}
	vsBuffer->Release();
}

void Device::InitPixelShader(char* FilePath, char* Entry, char* ShaderModel)
{
	ID3DBlob*	psBuffer	= 0;
	if(!CompileShader(FilePath, Entry, ShaderModel, &psBuffer))
	{
		MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
		return;
	}
	if(FAILED(m_d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &m_PixelShader)))
	{
		psBuffer->Release();
		DXTRACE_MSG("Error creating pixel shader!");
		return;
	}
	psBuffer->Release();
}

void Device::BeginScene(float fColor[4])
{
	m_d3dContext->ClearRenderTargetView(m_BackBufferTarget, fColor);
	m_d3dContext->IASetInputLayout(m_InputLayout);
	m_d3dContext->VSSetShader(m_VertexShader, 0, 0);
	m_d3dContext->PSSetShader(m_PixelShader, 0, 0);
}

void Device::EndScene()
{
	m_SwapChain->Present(0, 0);
}

XMMATRIX Device::GetVPMatrix()
{
	return XMMatrixOrthographicOffCenterLH(m_kCameraPos.x, m_iWidth + m_kCameraPos.x, m_kCameraPos.y,
											m_iHeight + m_kCameraPos.y, 0.1f, 100.0f);
}