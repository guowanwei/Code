#include "stdafx.h"
#include "Device.h"
#include "RenderTarget.h"
#include "Vector"
bool Device::init(HWND hwnd)
{
	RECT dimensions;
	GetClientRect(hwnd, &dimensions);
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
	};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;// DXGI_FORMAT_R16G16B16A16_FLOAT; //DXGI_FORMAT_R16G16B16A16_UNORM;// DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	unsigned int creationFlags = 0;
	//#ifdef _DEBUG
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//#endif
	HRESULT result;
	unsigned int driver = 0;
	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
			featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain_,
			&d3dDevice_, &featureLevel_, &d3dContext_);
		if (SUCCEEDED(result))
		{
			driverType_ = driverTypes[driver];
			break;
		}
	}
	if (FAILED(result))
	{
		//DXTRACE_MSG( "Failed to create the Direct3D device!" );
		return false;
	}
	ID3D11Texture2D* backBufferTexture;
	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if (FAILED(result))
	{
		//DXTRACE_MSG( "Failed to get the swap chain back buffer!" );
		return false;
	}
	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget_);
	if (backBufferTexture)
		backBufferTexture->Release();
	if (FAILED(result))
	{
		//DXTRACE_MSG( "Failed to create the render target view!" );
		return false;
	}

	// add depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	d3dDevice_->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	d3dDevice_->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, mDepthStencilView);
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dContext_->RSSetViewports(1, &viewport);

	mWinWidth = width;
	mWinHeight = height;

	return true;
}
bool Device::DrawOnScreenFinally()
{
	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, mDepthStencilView);
	ClearRenderTarget();
	return true;
}

bool Device::SetRenderTargets(const std::vector<RenderTarget*>& RenderTargets, ID3D11DepthStencilView* DepthBuffer)
{
	if (!DepthBuffer) DepthBuffer = mDepthStencilView;
	std::vector<ID3D11RenderTargetView*> Views;
	for (int i = 0; i < RenderTargets.size(); ++i)
	{
		Views.push_back(RenderTargets[i]->GetRenderTargetView());
	}
	d3dContext_->OMSetRenderTargets(Views.size(), &Views[0], DepthBuffer);
	return true;
}
Device::~Device()
{
	if (backBufferTarget_) backBufferTarget_->Release();
	if (swapChain_) swapChain_->Release();
	if (d3dContext_) d3dContext_->Release();
	if (d3dDevice_) d3dDevice_->Release();

	if (mDepthStencilBuffer) mDepthStencilBuffer->Release();
	if (mDepthStencilView) mDepthStencilView->Release();

	backBufferTarget_ = 0;
	swapChain_ = 0;
	d3dContext_ = 0;
	d3dDevice_ = 0;

	mDepthStencilBuffer = 0;
	mDepthStencilView = 0;
}